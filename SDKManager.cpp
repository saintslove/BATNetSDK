/*
 * SDKManager.cpp
 *
 *  Created on: 2016年8月11日
 *      Author: wong
 */

#include "SDKManager.h"

#include <time.h>
#include <stdlib.h>

#include "base/Logging.h"

#include "Common.h"
#include "ServerBase.h"
#include "CommonServer.h"

SDKManager::SDKManager(int32_t nDevType, std::string& strDevSN,  bool bAutoLogin)
: m_deviceType(nDevType)
, m_deviceSN(strDevSN)
, m_bAutoLogin(bAutoLogin)
{
    srandom(time(NULL));
}

SDKManager::~SDKManager()
{
    for (std::map<uint32_t, ServerBase*>::iterator it = m_mapInstance.begin();
        it != m_mapInstance.end(); ++it)
    {
        SAFE_DELETE(it->second);
    }
    m_mapInstance.clear();
    for (std::map<uint32_t, CommonServer*>::iterator it = m_mapInstance2.begin();
        it != m_mapInstance2.end(); ++it)
    {
        SAFE_DELETE(it->second);
    }
    m_mapInstance2.clear();
}

uint32_t SDKManager::GenerateId(bool bRaw)
{
    uint32_t id = 0;
    if (bRaw)
    {
        do
        {
            id = random() | 0x80000000;
        } while (id == 0 || m_mapInstance.find(id) != m_mapInstance.end());
    }
    else
    {
        do
        {
            id = random() & 0x7FFFFFFF;
        } while (id == 0 || m_mapInstance2.find(id) != m_mapInstance2.end());
    }
    return id;
}

int SDKManager::NewClient(int32_t nProtocolType,
        const std::string& serverIp, uint16_t serverPort, bool bRaw)
{
/*    switch (key)
    {
    case value:

        break;
    default:
        break;
    }*/
    uint32_t id = GenerateId(bRaw);
    if (bRaw)
    {
        m_mapInstance[id] = new ServerBase(serverIp, serverPort, false, 0, 1);
    }
    else
    {
        m_mapInstance2[id] = new CommonServer(serverIp, serverPort, false, 0, 1, m_deviceSN, m_deviceType);
    }
    return id;
}

int SDKManager::NewServer(int32_t nProtocolType,
        const std::string& localIp, uint16_t listenPort, bool bRaw)
{
    uint32_t id = GenerateId(bRaw);
    if (bRaw)
    {
        m_mapInstance[id] = new ServerBase(localIp, listenPort, true, 0, 65536);
    }
    else
    {
        m_mapInstance2[id] = new CommonServer(localIp, listenPort, true, 0, 65536);
    }
    return id;
}

int SDKManager::ReleaseObj(int id)
{
    if (id & 0x80000000)
    {
        if (m_mapInstance.find(id) == m_mapInstance.end())
        {
            return -1; //EC_InvalidParam;
        }
        SAFE_DELETE(m_mapInstance[id]);
        m_mapInstance.erase(id);
    }
    else
    {
        if (m_mapInstance2.find(id) == m_mapInstance2.end())
        {
            return -1; //EC_InvalidParam;
        }
        SAFE_DELETE(m_mapInstance2[id]);
        m_mapInstance2.erase(id);
    }
    return 0;
}

ServerBase* SDKManager::GetObj(int id)
{
    if (m_mapInstance.find(id) == m_mapInstance.end())
    {
        return NULL;
    }
    return m_mapInstance[id];
}

CommonServer* SDKManager::GetObj2(int id)
{
    if (m_mapInstance2.find(id) == m_mapInstance2.end())
    {
        return NULL;
    }
    return m_mapInstance2[id];
}
