/*
 * CommonServer.h
 *
 *  Created on: 2016年7月13日
 *      Author: wong
 */

#ifndef COMMONSERVER_H_
#define COMMONSERVER_H_

#include <queue>
#include <map>

#include "base/Thread.h"
#include "base/Condition.h"
#include "net/EventLoop.h"

#include "CCMS.h"

class Package;
class LoginPackage;
class LogoutPackage;
class ServerBase;

typedef int (MESSAGE_CALLBACK)(int sessionId, int msgId, const char* buf, int len, void* userdata);
typedef int (CONNECT_CALLBACK)(int sessionId, const char* sn, int status, void* userdata);
typedef int (DATAFLOW_CALLBACK)(int sessionId, const char* sn, int recvbytes, void* userdata);

class CommonServer
{
public:
    CommonServer(const std::string& ip, uint16_t port, bool bServer,
        uint32_t numThreads = 0, uint32_t maxConns = 0,
        const std::string& sn = "", int devType = CCMS_DEVTYPE_UNKNOWN);
    virtual ~CommonServer();

public:
    int Start();
    int SetMsgCallback(MESSAGE_CALLBACK* callback, void* pUserData);
    int SetConnCallback(CONNECT_CALLBACK* callback, void* pUserData);
    int SetDataFlowCallback(DATAFLOW_CALLBACK* callback, void* pUserData);
    int SendData(uint32_t sessionId, uint32_t msgId, const char* buf, size_t len);
    int SendData(const std::string& sn, uint32_t msgId, const char* buf, size_t len);
    int SendData(emDevType devType, uint32_t msgId, const char* buf, size_t len);
    int SendCommon(uint32_t sessionId, uint32_t msgId, uint32_t result);
    int SendLogin(uint32_t sessionId, const std::string& username, const std::string& password);
    int SendLoginResp(uint32_t sessionId, uint32_t result);
    int SendLogout(uint32_t sessionId, const std::string& username);
    int SendLogoutResp(uint32_t sessionId, uint32_t result);
    int SendKeepAlive(uint32_t sessionId);

private:
    void ProcessThreadFunc(); // work thread
    void ProcessMsg(uint32_t sessionId, Package* package);

private:
    static int RawConnCB(int sessionId, int status, const char* ip, unsigned short port, void* userdata)
    {
        CommonServer* that = reinterpret_cast<CommonServer*>(userdata);
        return that->OnRawConn(sessionId, status, ip, port);
    }
    int OnRawConn(int sessionId, int status, const char* ip, unsigned short port);
    static int RawDataFlowCB(int sessionId, int recvbytes, void* userdata)
    {
        CommonServer* that = reinterpret_cast<CommonServer*>(userdata);
        return that->OnRawDataFlow(sessionId, recvbytes);
    }
    int OnRawDataFlow(int sessionId, int recvbytes);
    static int RawMsgCB(int sessionId, const char* buf, int len, void* userdata)
    {
        CommonServer* that = reinterpret_cast<CommonServer*>(userdata);
        return that->OnRawMsg(sessionId, buf, len);
    }
    int OnRawMsg(int sessionId, const char* buf, int len);
    int ParsePackage(const char* buf, int len, Package** pPackage);
    Package* CreatePackage(uint32_t msgId);

private:
    void OnLogin(uint32_t sessionId, LoginPackage* package);
    void OnLogout(uint32_t sessionId, LogoutPackage* package);

private:
    void ResizeBuffer(size_t size);
    void ReleaseBuffer();
    int SendImpl(uint32_t sessionId, Package& package, size_t len);
    struct PeerAttr;
    PeerAttr* GetPeerAttr(uint32_t seesionId);
    std::string RemovePeerAttr(uint32_t seesionId);

private:
    struct PeerAttr
    {
        uint32_t seqno;
        std::string sn;
        int devType;
    };
    typedef std::map<uint32_t, PeerAttr> MapPeer;
    class PeerAttrSNFinder
    {
    public:
        PeerAttrSNFinder(const std::string& sn)
            : m_sn(sn)
        {
        }
        bool operator ()(const MapPeer::value_type& pair)
        {
            return pair.second.sn == m_sn;
        }
    private:
        const std::string& m_sn;
    };
    class PeerAttrTypeFinder
    {
    public:
        PeerAttrTypeFinder(int devType)
            : m_devType(devType)
        {
        }
        bool operator ()(const MapPeer::value_type& pair)
        {
            return pair.second.devType == m_devType;
        }
    private:
        int m_devType;
    };

private:
    bool m_bServer;
    std::string m_ip;
    uint16_t m_port;
    //uint32_t m_numThreads;
    uint32_t m_maxConns;
    std::string m_sn;
    emDevType m_devType;
    muduo::Thread m_threadProcess;
    bool m_bProcessThreadRunning;

    MapPeer m_mapPeer;

    struct PackageAttr
    {
        Package* package;
        uint32_t sessionId;
    };
    muduo::MutexLock m_mutexMsg;
    muduo::Condition m_condMsg;
    std::queue<PackageAttr> m_packgeQueue;

    muduo::net::EventLoop* m_loop;

    MESSAGE_CALLBACK* m_pfnOnRecv;
    CONNECT_CALLBACK* m_pfnOnConn;
    DATAFLOW_CALLBACK* m_pfnOnDataFlow;
    void* m_pUserData;

    char* m_bufferPool;
    size_t m_bufferPoolSize;

    ServerBase* m_netBase;

private:
    CommonServer(const CommonServer& other);
};

#endif /* COMMONSERVER_H_ */
