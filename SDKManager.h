/*
 * SDKManager.h
 *
 *  Created on: 2016年8月11日
 *      Author: wong
 */

#ifndef SDKMANAGER_H_
#define SDKMANAGER_H_

#include <stdint.h>
#include <string>
#include <map>

#define CHECK_RAW(id) \
    if (!(id >> (sizeof(id) * 8 - 1))) \
    { \
        return -1; \
    }

#define CHECK_NEW(id) \
    if (id >> (sizeof(id) * 8 - 1)) \
    { \
        return -1; \
    }

class ServerBase;
class CommonServer;

class SDKManager
{
public:
    SDKManager(int32_t nDevType, std::string& strDevSN,  bool bAutoLogin);
    virtual ~SDKManager();

public:
    int NewClient(int32_t nProtocolType,
        const std::string& serverIp, uint16_t serverPort, bool bRaw);
    int NewServer(int32_t nProtocolType,
        const std::string& localIp, uint16_t listenPort, bool bRaw);
    int ReleaseObj(int id);
    ServerBase* GetObj(int id);
    CommonServer* GetObj2(int id);

private:
    uint32_t GenerateId(bool bRaw);

private:
    int32_t m_deviceType;
    std::string m_deviceSN;
    bool m_bAutoLogin;

private:
    std::map<uint32_t, ServerBase*> m_mapInstance;
    std::map<uint32_t, CommonServer*> m_mapInstance2;
};

#endif /* SDKMANAGER_H_ */
