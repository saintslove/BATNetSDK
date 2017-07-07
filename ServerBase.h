/*
 * ServerBase.h
 *
 *  Created on: 2016年7月13日
 *      Author: wong
 */

#ifndef SERVERBASE_H_
#define SERVERBASE_H_

#include <queue>

#include "base/Thread.h"
#include "base/Condition.h"
#include "net/EventLoop.h"
#include "net/TcpServer.h"

//#include "BATNetSDKRawAPI.h"
typedef int (RAWMESSAGE_CALLBACK)(int sessionId, const char* buf, int len, void* userdata);
typedef int (RAWCONNECT_CALLBACK)(int sessionId, int status, const char* ip, unsigned short port, void* userdata);
typedef int (RAWDATAFLOW_CALLBACK)(int sessionId, int recvbytes, void* userdata);

class ServerBase
{
    typedef muduo::net::TcpConnectionPtr TcpConnPtr;
    typedef muduo::net::Buffer Buffer;
public:
    ServerBase(const std::string& ip, uint16_t port, bool bServer,
        uint32_t numThreads = 0, uint32_t maxConns = 0);
    virtual ~ServerBase();

public:
    int Start();
    int SetRawMsgCallback(RAWMESSAGE_CALLBACK* callback, void* pUserData);
    int SetConnCallback(RAWCONNECT_CALLBACK* callback, void* pUserData);
    int SetDataFlowCallback(RAWDATAFLOW_CALLBACK* callback, void* pUserData);
    int Send(uint32_t sessionId, const char* buf, size_t len);
    int SendAll(const char* buf, size_t len);
    int Disconnect(uint32_t sessionId);
    void RemoveConn(uint32_t sessionId);

private:
    void NetworkThreadFunc(); // network thread
    void MonitorThreadFunc(); // monitor thread
    void OnConnection(const TcpConnPtr& conn);
    void OnMessage(const TcpConnPtr& conn,
        Buffer* buf, muduo::Timestamp time);

private:
    void UpdateRecvTime(uint32_t id);
    void UpdateRecvBytes(uint32_t id, uint32_t bytes);

private:
    bool m_bServer;
    std::string m_ip;
    uint16_t m_port;
    //uint32_t m_numThreads;
    uint32_t m_maxConns;
    muduo::Thread m_threadNetwork;
    muduo::Thread m_threadMonitor;
    bool m_bNetworkThreadRunning;
    bool m_bMonitorThreadRunning;

    struct ConnAttr
    {
        TcpConnPtr conn;
        muduo::Timestamp connectTime;
        muduo::Timestamp recvTime;
        muduo::Timestamp sendTime;
        muduo::Timestamp fromTimeAddUp;
        uint32_t recvBytesAddUp;
    };
    typedef std::map<uint32_t, ConnAttr> MapConn;

    muduo::MutexLock m_mutexConn;
    std::queue<uint32_t> m_availIds;
    MapConn m_mapConn;

    muduo::net::EventLoop* m_loop;

    RAWMESSAGE_CALLBACK* m_pfnOnRecvRaw;
    RAWCONNECT_CALLBACK* m_pfnOnConn;
    RAWDATAFLOW_CALLBACK* m_pfnOnDataFlow;
    void* m_pUserData;

private:
    ServerBase(const ServerBase& other);
};

#endif /* SERVERBASE_H_ */
