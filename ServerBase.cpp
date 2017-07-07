/*
 * ServerBase.cpp
 *
 *  Created on: 2016年7月13日
 *      Author: wong
 */

#include "ServerBase.h"

#include <boost/bind.hpp>

#include "base/Logging.h"
#include "base/Condition.h"
#include "net/EventLoop.h"
#include "net/TcpServer.h"
#include "net/TcpClient.h"

#include "common.h"
#include "Common.h"

ServerBase::ServerBase(const std::string& ip, uint16_t port, bool bServer,
    uint32_t numThreads/* = 0*/, uint32_t maxConns/* = 0*/)
: m_bServer(bServer)
, m_ip(ip)
, m_port(port)
//, m_numThreads(numThreads)
, m_maxConns(maxConns)
, m_threadNetwork(
    boost::bind(&ServerBase::NetworkThreadFunc, this), "NetworkThread")
, m_threadMonitor(
    boost::bind(&ServerBase::MonitorThreadFunc, this), "MonitorThread")
, m_bNetworkThreadRunning(true)
, m_bMonitorThreadRunning(true)
, m_mutexConn()
, m_loop(NULL)
, m_pfnOnRecvRaw(NULL)
, m_pfnOnConn(NULL)
, m_pfnOnDataFlow(NULL)
, m_pUserData(NULL)
{
    assert(m_maxConns > 0);
    for (uint32_t i = 0; i < m_maxConns; ++i)
    {
        m_availIds.push(i + 1);
    }
}

ServerBase::~ServerBase()
{
    m_bNetworkThreadRunning = false;
    m_bMonitorThreadRunning = false;
    m_threadNetwork.join();
    m_threadMonitor.join();
}

int ServerBase::Start()
{
    m_threadNetwork.start();
    m_threadMonitor.start();
    return 0;
}

void ServerBase::NetworkThreadFunc()
{
    LOG_INFO << "Pid = " << getpid() << ", Tid = " << muduo::CurrentThread::tid();
    while (m_bNetworkThreadRunning)
    {
        SAFE_DELETE(m_loop);
        if (m_bServer)
        {
            m_loop = new muduo::net::EventLoop();
            muduo::net::InetAddress listenAddr(m_port);
            muduo::net::TcpServer server(m_loop, listenAddr, "ServerBase");
            server.setConnectionCallback(
                boost::bind(&ServerBase::OnConnection, this, _1));
            server.setMessageCallback(
                boost::bind(&ServerBase::OnMessage, this, _1, _2, _3));
            server.start();
            m_loop->loop();
        }
        else
        {
            m_loop = new muduo::net::EventLoop();
            muduo::net::InetAddress serverAddr(m_ip, m_port);
            muduo::net::TcpClient client(m_loop, serverAddr, "ClientBase");
            client.setConnectionCallback(
                boost::bind(&ServerBase::OnConnection, this, _1));
            client.setMessageCallback(
                boost::bind(&ServerBase::OnMessage, this, _1, _2, _3));
            client.connect();
            m_loop->loop();
        }
    }
}

void ServerBase::MonitorThreadFunc()
{
    while (m_bMonitorThreadRunning)
    {
        sleep(60); //60s
        LOG_INFO << "ServerBase::MonitorThreadFunc() checking";
        muduo::MutexLockGuard lock(m_mutexConn);
        for (MapConn::iterator it = m_mapConn.begin(); it != m_mapConn.end();)
        {
            if (timeDifference(muduo::Timestamp::now(), it->second.fromTimeAddUp) >= 60 * 5)
            {
                if (m_pfnOnDataFlow != NULL)
                {
                    m_pfnOnDataFlow(it->first, it->second.recvBytesAddUp, m_pUserData);
                }
                it->second.fromTimeAddUp = muduo::Timestamp::now();
                it->second.recvBytesAddUp = 0;
            }

            if (m_pfnOnConn != NULL)
            {
                const muduo::net::InetAddress& addr = it->second.conn->peerAddress();
                if (timeDifference(muduo::Timestamp::now(), it->second.recvTime) > 60 * 3)
                {
                    if (m_pfnOnConn(it->first, CS_Timeout, addr.toIp().c_str(), addr.toPort(), m_pUserData) == -1)
                    {
                        it->second.conn->forceClose();
                        m_availIds.push(it->first);
                        m_mapConn.erase(it++);
                        continue;
                    }
                    else
                    {
                        it->second.recvTime = muduo::Timestamp::now();
                    }
                }

                if (timeDifference(muduo::Timestamp::now(), it->second.sendTime) >= 60)
                {
                    m_pfnOnConn(it->first, CS_TimeoutOnce, addr.toIp().c_str(), addr.toPort(), m_pUserData);
                }
            }

            it++;
        }
    }
}

void ServerBase::OnConnection(const TcpConnPtr& conn)
{
    if (conn->connected())
    {
        LOG_INFO << "new connection [ " << conn->name()
            << " ] from " << conn->peerAddress().toIpPort();

        muduo::MutexLockGuard lock(m_mutexConn);
        if (!m_availIds.empty())
        {
            uint32_t id = m_availIds.front();
            m_availIds.pop();
            conn->setContext(id);
            m_mapConn[id].conn = conn;
            m_mapConn[id].connectTime = muduo::Timestamp::now();
            m_mapConn[id].recvTime = muduo::Timestamp::now();
            m_mapConn[id].sendTime = muduo::Timestamp::now();
            m_mapConn[id].fromTimeAddUp = muduo::Timestamp::now();
            m_mapConn[id].recvBytesAddUp = 0;
            LOG_INFO << "ServerBase::OnConnection id = " << id
                << " count = " << m_mapConn.size();
            if (m_pfnOnConn != NULL)
            {
                int ret = m_pfnOnConn(id, CS_Connected,
                    conn->peerAddress().toIp().c_str(),
                    conn->peerAddress().toPort(), m_pUserData);
                if (ret == -1)
                {
                    m_availIds.push(id);
                    m_mapConn.erase(id);
                    conn->forceClose();
                    LOG_INFO << "ServerBase::OnConnection Disconnect by user!";
                }
            }
        }
        else
        {
            LOG_ERROR << "Over the limit of Max Conns!!!";
            conn->forceClose();
        }
    }
    else
    {
        LOG_INFO << "connection [ " << conn->name().c_str() << " ] is Down";
        if (!conn->getContext().empty())
        {
            uint32_t id = boost::any_cast<uint32_t>(conn->getContext());
            LOG_INFO << "ServerBase::OnConnection down id = " << id;
            if (id > 0 && id <= m_maxConns)
            {
                if (m_pfnOnConn != NULL)
                {
                    m_pfnOnConn(id, CS_Disconnect, conn->peerAddress().toIp().c_str(),
                        conn->peerAddress().toPort(), m_pUserData);
                }
                muduo::MutexLockGuard lock(m_mutexConn);
                m_availIds.push(id);
                m_mapConn.erase(id);
            }
        }
        //PrintMap();
        if (!m_bServer)
        {
            m_loop->quit();
        }
    }
}

void ServerBase::OnMessage(const TcpConnPtr& conn,
        Buffer* buf, muduo::Timestamp time)
{
    LOG_TRACE << "peer = " << conn->peerAddress().toIpPort().c_str()
        << " buffer len = " << buf->readableBytes();

    if (conn->getContext().empty())
    {
        LOG_ERROR << "receive msg from invalid conn!";
        buf->retrieveAll();
        return;
    }
    uint32_t id = boost::any_cast<uint32_t>(conn->getContext());
    UpdateRecvTime(id);

    if (m_pfnOnRecvRaw == NULL)
    {
        LOG_ERROR << "m_pfnOnRecvRaw is NULL!";
        buf->retrieveAll();
        return;
    }

    while (buf->readableBytes() > 0)
    {
        int recvLen = m_pfnOnRecvRaw(id, buf->peek(), buf->readableBytes(), m_pUserData);
        if (recvLen > 0 && (size_t)recvLen <= buf->readableBytes())
        {
            UpdateRecvBytes(id, recvLen);
            buf->retrieve(recvLen);
        }
        else if (recvLen == 0)
        {
            LOG_DEBUG << "Uncompleted package, continue recv. readableBytes() = "
                << buf->readableBytes();
            break;
        }
        else if (recvLen < 0)
        {
            buf->retrieveAll();
        }
        else
        {
            buf->retrieveAll();
            LOG_ERROR << "recvLen = " << recvLen << " readableBytes() = "
                << buf->readableBytes();
        }
    }
}

void ServerBase::UpdateRecvTime(uint32_t id)
{
    muduo::MutexLockGuard lock(m_mutexConn);
    MapConn::iterator it = m_mapConn.find(id);
    if (it != m_mapConn.end())
    {
        it->second.recvTime = muduo::Timestamp::now();
    }
}

void ServerBase::UpdateRecvBytes(uint32_t id, uint32_t bytes)
{
    muduo::MutexLockGuard lock(m_mutexConn);
    MapConn::iterator it = m_mapConn.find(id);
    if (it != m_mapConn.end())
    {
        it->second.recvBytesAddUp += bytes;
    }
}

void ServerBase::RemoveConn(uint32_t sessionId)
{
    muduo::MutexLockGuard lock(m_mutexConn);
    MapConn::iterator it = m_mapConn.find(sessionId);
    if (it != m_mapConn.end())
    {
        std::string ip = it->second.conn->peerAddress().toIp().c_str();
        short port = it->second.conn->peerAddress().toPort();
        it->second.conn->forceClose();
        m_availIds.push(sessionId);
        m_mapConn.erase(it);
        if (m_pfnOnConn != NULL)
        {
            m_pfnOnConn(sessionId, CS_Disconnect, ip.c_str(), port, m_pUserData);
        }
    }
}

int ServerBase::SetRawMsgCallback(RAWMESSAGE_CALLBACK* callback, void* pUserData)
{
    if (callback == NULL)
    {
        return EC_InvalidParam;
    }
    m_pfnOnRecvRaw = callback;
    m_pUserData = pUserData;
    return EC_Ok;
}

int ServerBase::SetConnCallback(RAWCONNECT_CALLBACK* callback, void* pUserData)
{
    if (callback == NULL)
    {
        return EC_InvalidParam;
    }
    m_pfnOnConn = callback;
    m_pUserData = pUserData;
    return EC_Ok;
}

int ServerBase::SetDataFlowCallback(RAWDATAFLOW_CALLBACK* callback, void* pUserData)
{
    if (callback == NULL)
    {
        return EC_InvalidParam;
    }
    m_pfnOnDataFlow = callback;
    m_pUserData = pUserData;
    return EC_Ok;
}

int ServerBase::Send(uint32_t sessionId, const char* buf, size_t len)
{
    ConnAttr* connAttr = NULL;
    if (!m_bServer)
    {
        //muduo::MutexLockGuard lock(m_mutexConn); // wbf.mark  加锁范围值得商榷
        if (m_mapConn.empty())
        {
            return EC_InvalidState;
        }
        connAttr = &m_mapConn.begin()->second;
    }
    else
    {
        //muduo::MutexLockGuard lock(m_mutexConn);
        MapConn::iterator it = m_mapConn.find(sessionId);
        if (it == m_mapConn.end())
        {
            return EC_InvalidParam;
        }
        connAttr = &it->second;
    }
    if (connAttr == NULL || connAttr->conn == NULL
        || !connAttr->conn->connected())
    {
        return EC_Fail;
    }

    connAttr->conn->send(buf, len);
    connAttr->sendTime = muduo::Timestamp::now();
    return EC_Ok;
}

int ServerBase::SendAll(const char* buf, size_t len)
{
    //muduo::MutexLockGuard lock(m_mutexConn); // wbf.mark  加锁范围值得商榷
    for (MapConn::iterator it = m_mapConn.begin(); it != m_mapConn.end(); ++it)
    {
        if (it->second.conn == NULL || !it->second.conn->connected())
        {
            continue;
        }
        it->second.conn->send(buf, len);
        it->second.sendTime = muduo::Timestamp::now();
    }
    return EC_Ok;
}

int ServerBase::Disconnect(uint32_t sessionId)
{
    if (m_bServer)
    {
        RemoveConn(sessionId);
    }
    else
    {
        if (!m_mapConn.empty())
        {
            RemoveConn(m_mapConn.begin()->first);
        }
    }
    return 0;
}
