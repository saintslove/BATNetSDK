/*
 * sdk.cpp
 *
 *  Created on: 2016年7月28日
 *      Author: wong
 */

#include "sdk.h"

#include <stddef.h>

#include "SDKManager.h"
#include "Common.h"

SDKManager* g_sdk = NULL;

int Init(int nDevType, char* strDevSN,  bool bAutoLogin/*=true*/)
{
    if (g_sdk != NULL)
    {
        return -1;
    }
    std::string sn(reinterpret_cast<char*>(strDevSN));
    static SDKManager mgr(nDevType, sn, bAutoLogin);
    g_sdk = &mgr;//new SDKManager(nDevType, sn, bAutoLogin);
    return 0;
}

void Release()
{
    //SAFE_DELETE(g_sdk);
}

int CreateClientObj(CCMS_NETADDR* pServerAddr, int nProtocolType, bool bRaw)
{
    const std::string& ip = ARRAY2STR(pServerAddr->chIP);
    return g_sdk->NewClient(nProtocolType, ip, pServerAddr->nPort, bRaw);
}

int CreateServerObj(CCMS_NETADDR* pListenAddr, int nProtocolType, bool bRaw)
{
    const std::string& ip = ARRAY2STR(pListenAddr->chIP);
    return g_sdk->NewServer(nProtocolType, ip, pListenAddr->nPort, bRaw);
}

int DeleteObj(int nHandle)
{
    return g_sdk->ReleaseObj(nHandle);
}
