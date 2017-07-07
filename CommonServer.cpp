/*
 * CommonServer.cpp
 *
 *  Created on: 2016年7月13日
 *      Author: wong
 */

#include "CommonServer.h"

#include <boost/bind.hpp>

#include "base/Logging.h"

#include "common.h"
#include "Common.h"
#include "Package/Header.h"
#include "Package/CommonPackage.h"
#include "ServerBase.h"

CommonServer::CommonServer(const std::string& ip, uint16_t port, bool bServer,
    uint32_t numThreads/* = 0*/, uint32_t maxConns/* = 0*/,
    const std::string& sn/* = ""*/, int devType/* = CCMS_DEVTYPE_UNKNOWN*/)
: m_bServer(bServer)
, m_ip(ip)
, m_port(port)
//, m_numThreads(numThreads)
, m_maxConns(maxConns)
, m_sn(sn)
, m_devType((emDevType)devType)
, m_threadProcess(
    boost::bind(&CommonServer::ProcessThreadFunc, this), "ProcessThread")
, m_bProcessThreadRunning(true)
, m_mutexMsg()
, m_condMsg(m_mutexMsg)
, m_loop(NULL)
, m_pfnOnRecv(NULL)
, m_pfnOnConn(NULL)
, m_pfnOnDataFlow(NULL)
, m_pUserData(NULL)
, m_bufferPool(NULL)
, m_bufferPoolSize(0)
, m_netBase(NULL)
{
    ResizeBuffer(1024);
    m_netBase = new ServerBase(ip, port, bServer, numThreads, maxConns);
    m_netBase->SetRawMsgCallback(RawMsgCB, this);
    m_netBase->SetConnCallback(RawConnCB, this);
    m_netBase->SetDataFlowCallback(RawDataFlowCB, this);

}

CommonServer::~CommonServer()
{
    m_bProcessThreadRunning = false;
    m_threadProcess.join();
    SAFE_DELETE(m_netBase);
    ReleaseBuffer();
}

int CommonServer::Start()
{
    m_threadProcess.start();
    return m_netBase->Start();
}

void CommonServer::ProcessThreadFunc()
{
    while (m_bProcessThreadRunning)
    {
        PackageAttr attr;
        {
            muduo::MutexLockGuard lock(m_mutexMsg);
            if (m_packgeQueue.empty())
            {
                m_condMsg.waitForMillSeconds(10);
                continue;
            }
            else
            {
                LOG_DEBUG << "m_packgeQueue.size() = " << m_packgeQueue.size();
                attr = m_packgeQueue.front();
                m_packgeQueue.pop();
            }
        }

        assert(attr.package != NULL);
        ProcessMsg(attr.sessionId, attr.package);
        SAFE_DELETE(attr.package);
    }
}

int CommonServer::OnRawConn(int sessionId, int status, const char* ip, unsigned short port)
{
    LOG_INFO << "CommonServer::OnRawConn() " << sessionId << " " << status
        << " " << ip << ":" << port;
    if (m_pfnOnConn != NULL && status != CS_TimeoutOnce)
    {
        PeerAttr* peer = GetPeerAttr(sessionId);
        std::string sn = (peer == NULL) ? "" : peer->sn;
        m_pfnOnConn(sessionId, sn.c_str(), status, m_pUserData);
    }

    switch (status)
    {
    case CS_Connected:
        if (!m_bServer)
        {
            PeerAttr peer = {0, "", DT_Unknown};
            m_mapPeer[sessionId] = peer;
            SendLogin(sessionId, m_sn, m_sn);
        }
        break;
    case CS_TimeoutOnce:
        SendKeepAlive(sessionId);
        break;
    case CS_Disconnect:
    case CS_Logout:
    case CS_Timeout:
        {
            std::string sn = RemovePeerAttr(sessionId);
            LOG_INFO << "conn broken. id=" << sessionId << " sn=" << sn
                << " status=" << status << " " << ip << ":" << port;
        }
        break;
    }

    return (status == CS_Timeout) ? -1 : 0;// wbf.mark
}

int CommonServer::OnRawDataFlow(int sessionId, int recvbytes)
{
    if (m_pfnOnDataFlow != NULL)
    {
        PeerAttr* peer = GetPeerAttr(sessionId);
        std::string sn = (peer == NULL) ? "" : peer->sn;
        m_pfnOnDataFlow(sessionId, sn.c_str(), recvbytes, m_pUserData);
    }
    return 0;
}

int CommonServer::OnRawMsg(int sessionId, const char* buf, int len)
{
    Package* package = NULL;
    int ret = ParsePackage(buf, len, &package);
    if (ret == EC_Continue)
    {
        LOG_TRACE << "Incomplete Packet, continue receive from "
            << sessionId << ", current len = " << len;
        return 0;
    }
    else if (ret == EC_Fail)
    {
        LOG_ERROR << "Invalid Packet, retrieveAll!";
        return -1;
    }
    else if (ret == EC_Ok && package != NULL)
    {
        LOG_TRACE << "Complete Packet, " << package->GetName()
            << "[" << package->GetLength() << "] from " << sessionId;
        //LOG_INFO << package->HeaderToString();
        PackageAttr attr = {package, (uint32_t)sessionId};
        muduo::MutexLockGuard lock(m_mutexMsg);
        m_packgeQueue.push(attr);
        m_condMsg.notify();
        return package->GetLength();
    }
    else
    {
        LOG_ERROR << "OnMessage ret = " << ret << " package = " << package;
        SAFE_DELETE(package);
        assert(false);
        return -1;
    }
}

int CommonServer::ParsePackage(const char* buf, int len, Package** package)
{
    assert(package != NULL && *package == NULL);
    if (package == NULL)
    {
        return EC_Fail;
    }
    uint32_t msgId = MSG_Unknown;
    int ret = Package::PeekPackage(buf, len, msgId);
    if (ret != EC_Ok)
    {
        return ret;
    }

    *package = CreatePackage(msgId);
    if (*package == NULL)
    {
        return EC_Fail;
    }
    ret = (*package)->Decode(buf, len);
    if (ret != EC_Ok)
    {
        SAFE_DELETE(*package);
    }
    return ret;
}

Package* CommonServer::CreatePackage(uint32_t msgId)
{
    switch (msgId)
    {
    case MSG_Login:
        return new LoginPackage();
    case MSG_LoginResp:
        return new LoginRespPackage();
    case MSG_Logout:
        return new LogoutPackage();
    case MSG_LogoutResp:
        return new LogoutRespPackage();
    case MSG_KeepAlive:
        return new KeepAlivePackage();
    case MSG_FEPCommonResp:
        //return new FEPCommonRespPackage();
    case MSG_PlatformCommonResp:
        //return new PlatformCommonRespPackage();
    default:
        return new CommonPackage();
    }
}

void CommonServer::ProcessMsg(uint32_t sessionId, Package* package)
{
    assert(package != NULL);
    switch (package->header.u4MessageId)
    {
    case MSG_Login:
        OnLogin(sessionId, reinterpret_cast<LoginPackage*>(package));
        break;
    case MSG_LoginResp:
        break;
    case MSG_Logout:
        OnLogout(sessionId, reinterpret_cast<LogoutPackage*>(package));
        break;
    case MSG_LogoutResp:
        break;
    case MSG_KeepAlive:
        break;
    default:
        if (m_pfnOnRecv != NULL)
        {
            //ProcessMsgEx(package);
            CommonPackage* p = reinterpret_cast<CommonPackage*>(package);
            m_pfnOnRecv(sessionId, p->header.u4MessageId,
                reinterpret_cast<char*>(p->u1DataPtr),
                p->header.u4BodyLen, m_pUserData);
        }
        break;
    }
}

void CommonServer::OnLogin(uint32_t sessionId, LoginPackage* package)
{
    const std::string& username = ARRAY2STR(package->username);
    LOG_INFO << "CommonServer::OnLogin username = " << username;
    if (!username.empty())
    {
        //muduo::MutexLockGuard lock(m_mutexConn);
        MapPeer::iterator it = m_mapPeer.find(sessionId);
        MapPeer::iterator it2 = std::find_if(m_mapPeer.begin(),
            m_mapPeer.end(), PeerAttrSNFinder(username));
        if (it != m_mapPeer.end() && it2 != m_mapPeer.end() && it != it2)
        {
            LOG_INFO << " username conflict. " << username
                << " session1=" << it->first << " session2=" << it2->first;
            m_mapPeer.erase(it2);
            m_netBase->RemoveConn(it2->first);
        }
        m_mapPeer[sessionId].sn = username;
        m_mapPeer[sessionId].devType = package->header.u2DeviceType;

        if (m_pfnOnConn != NULL)
        {
            m_pfnOnConn(sessionId, username.c_str(), CS_Login, m_pUserData);
        }
    }
    SendLoginResp(sessionId, username.empty()
        ? LoginRespPackage::LOGIN_UserNotExist : LoginRespPackage::LOGIN_Success);
}

void CommonServer::OnLogout(uint32_t sessionId, LogoutPackage* package)
{
    const std::string& username = ARRAY2STR(package->username);
    LOG_INFO << "CommonServer::OnLogout username = " << username;
    if (!username.empty())
    {
        //muduo::MutexLockGuard lock(m_mutexConn);
        MapPeer::iterator it = m_mapPeer.find(sessionId);
        if (it == m_mapPeer.end())
        {
            // log
        }
        else
        {
            m_mapPeer.erase(it);
            if (m_pfnOnConn != NULL)
            {
                m_pfnOnConn(sessionId, username.c_str(), CS_Logout, m_pUserData);
            }
            m_netBase->RemoveConn(sessionId);
        }
    }
    SendLogoutResp(sessionId, username.empty()
        ? LogoutRespPackage::LOGOUT_UserNotExist : LogoutRespPackage::LOGOUT_Success);
}

void CommonServer::ResizeBuffer(size_t size)
{
    if (size > m_bufferPoolSize)
    {
        SAFE_DELETEA(m_bufferPool);
        m_bufferPool = new char[size];
        m_bufferPoolSize = size;
    }
}

void CommonServer::ReleaseBuffer()
{
    SAFE_DELETEA(m_bufferPool);
    m_bufferPoolSize = 0;
}

int CommonServer::SetMsgCallback(MESSAGE_CALLBACK* callback, void* pUserData)
{
    if (callback == NULL)
    {
        return EC_InvalidParam;
    }
    m_pfnOnRecv = callback;
    m_pUserData = pUserData;
    return EC_Ok;
}

int CommonServer::SetConnCallback(CONNECT_CALLBACK* callback, void* pUserData)
{
    if (callback == NULL)
    {
        return EC_InvalidParam;
    }
    m_pfnOnConn = callback;
    m_pUserData = pUserData;
    return EC_Ok;
}

int CommonServer::SetDataFlowCallback(DATAFLOW_CALLBACK* callback, void* pUserData)
{
    if (callback == NULL)
    {
        return EC_InvalidParam;
    }
    m_pfnOnDataFlow = callback;
    m_pUserData = pUserData;
    return EC_Ok;
}

CommonServer::PeerAttr* CommonServer::GetPeerAttr(uint32_t sessionId)
{
    if (!m_bServer)
    {
        //muduo::MutexLockGuard lock(m_mutexConn); // wbf.mark  加锁范围值得商榷
        if (m_mapPeer.empty())
        {
            return NULL;
        }
        return &m_mapPeer.begin()->second;
    }
    else
    {
        //muduo::MutexLockGuard lock(m_mutexConn);
        MapPeer::iterator it = m_mapPeer.find(sessionId);
        if (it == m_mapPeer.end())
        {
            return NULL;
        }
        return &it->second;
    }
}

std::string CommonServer::RemovePeerAttr(uint32_t sessionId)
{
    std::string sn = "";
    if (!m_bServer)
    {
        //muduo::MutexLockGuard lock(m_mutexConn); // wbf.mark  加锁范围值得商榷
        if (!m_mapPeer.empty() && m_mapPeer.begin()->first == sessionId)
        {
            sn = m_mapPeer.begin()->second.sn;
            m_mapPeer.clear();
        }
    }
    else
    {
        //muduo::MutexLockGuard lock(m_mutexConn);
        MapPeer::iterator it = m_mapPeer.find(sessionId);
        if (it != m_mapPeer.end())
        {
            sn = it->second.sn;
            m_mapPeer.erase(it);
        }
    }
    return sn;
}

int CommonServer::SendImpl(uint32_t sessionId, Package& package, size_t len)
{
    PeerAttr* peerAttr = GetPeerAttr(sessionId);
    if (peerAttr == NULL)
    {
        return EC_Fail;
    }

    package.header.u2DeviceType = m_devType;
    package.header.u4SeqNo = peerAttr->seqno++;

    ResizeBuffer(len);
    int ret = package.Encode(m_bufferPool, len);
    if (ret != EC_Ok)
    {
        return ret;
    }

    return m_netBase->Send(sessionId, m_bufferPool, len);
}

int CommonServer::SendData(uint32_t sessionId, uint32_t msgId, const char* buf, size_t len)
{
    CommonPackage package;
    package.header.u4MessageId = msgId;
    package.header.u4BodyLen = len;
    package.u1DataPtr = reinterpret_cast<uint8_t*>(const_cast<char*>(buf));

    return SendImpl(sessionId, package, package.GetLength());
}

int CommonServer::SendData(const std::string& sn, uint32_t msgId, const char* buf, size_t len)
{
    if (sn.empty())
    {
        return EC_Fail;
    }

    CommonPackage package;
    package.header.u4MessageId = msgId;
    package.header.u4BodyLen = len;
    package.u1DataPtr = reinterpret_cast<uint8_t*>(const_cast<char*>(buf));

    MapPeer::iterator it = std::find_if(m_mapPeer.begin(), m_mapPeer.end(), PeerAttrSNFinder(sn));
    if (it == m_mapPeer.end())
    {
        return EC_Fail;
    }
    return SendImpl(it->first, package, package.GetLength());
}

int CommonServer::SendData(emDevType devType, uint32_t msgId, const char* buf, size_t len)
{
    if (devType == CCMS_DEVTYPE_UNKNOWN)
    {
        return EC_Fail;
    }

    CommonPackage package;
    package.header.u4MessageId = msgId;
    package.header.u4BodyLen = len;
    package.u1DataPtr = reinterpret_cast<uint8_t*>(const_cast<char*>(buf));

    MapPeer::iterator it = std::find_if(m_mapPeer.begin(), m_mapPeer.end(), PeerAttrTypeFinder(devType));
    LOG_INFO << "CommonServer::SendData m_mapConn.size() = " << m_mapPeer.size();
    while (it != m_mapPeer.end())
    {
        SendImpl(it->first, package, package.GetLength());
        it = std::find_if(++it, m_mapPeer.end(), PeerAttrTypeFinder(devType));
    }
    return 0;
}

int CommonServer::SendCommon(uint32_t sessionId, uint32_t msgId, uint32_t result)
{
    Package package;
    package.header.u4MessageId = msgId;
    package.header.u4Reserved = result;

    return SendImpl(sessionId, package, package.GetLength());
}

int CommonServer::SendLogin(uint32_t sessionId, const std::string& username, const std::string& password)
{
    LoginPackage package;
    STR2ARRAY(username, package.username);
    STR2ARRAY(password, package.password);

    return SendImpl(sessionId, package, package.GetLength());
}

int CommonServer::SendLoginResp(uint32_t sessionId, uint32_t result)
{
    LoginRespPackage package;
    package.header.u4Reserved = result;

    return SendImpl(sessionId, package, package.GetLength());
}

int CommonServer::SendLogout(uint32_t sessionId , const std::string& username)
{
    LogoutPackage package;
    STR2ARRAY(username, package.username);

    return SendImpl(sessionId, package, package.GetLength());
}

int CommonServer::SendLogoutResp(uint32_t sessionId, uint32_t result)
{
    LogoutRespPackage package;
    package.header.u4Reserved = result;

    return SendImpl(sessionId, package, package.GetLength());
}

int CommonServer::SendKeepAlive(uint32_t sessionId)
{
    KeepAlivePackage package;

    return SendImpl(sessionId, package, package.GetLength());
}
