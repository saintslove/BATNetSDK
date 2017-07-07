/*
 * rawSdk.cpp
 *
 *  Created on: 2017年2月28日
 *      Author: wong
 */

#include "BATNetSDKRawAPI.h"

#include <stddef.h>

#include "SDKManager.h"
#include "ServerBase.h"
#include "sdk.h"

extern SDKManager* g_sdk;

#define bRaw true

/*BATNETSDK_API int BATNetSDK_Init(int nDevType, char* strDevSN, bool bAutoLogin = true)
{
    return Init(nDevType, strDevSN, bAutoLogin);
}

BATNETSDK_API void BATNetSDK_Release()
{
    Release();
}*/

BATNETSDK_API int BATNetSDKRaw_CreateClientObj(CCMS_NETADDR* pServerAddr, int nProtocolType)
{
    return CreateClientObj(pServerAddr, nProtocolType, bRaw);
}

BATNETSDK_API int BATNetSDKRaw_CreateServerObj(CCMS_NETADDR* pListenAddr, int nProtocolType)
{
    return CreateServerObj(pListenAddr, nProtocolType, bRaw);
}

BATNETSDK_API int BATNetSDKRaw_DeleteObj(int nHandle)
{
    return DeleteObj(nHandle);
}

BATNETSDK_API int BATNetSDKRaw_Start(int nHandle)
{
    CHECK_RAW(nHandle);
    ServerBase* obj = g_sdk->GetObj(nHandle);
    if (obj != NULL)
    {
        return obj->Start();
    }
    return -1;
}

BATNETSDK_API int BATNetSDKRaw_SetConnCallBack(int nHandle, RAWCONNECT_CALLBACK* callback, void* pUserData)
{
    CHECK_RAW(nHandle);
    ServerBase* obj = g_sdk->GetObj(nHandle);
    if (obj != NULL)
    {
        return obj->SetConnCallback(callback, pUserData);
    }
    return -1;
}

BATNETSDK_API int BATNetSDKRaw_SetDataFlowCallBack(int nHandle, RAWDATAFLOW_CALLBACK* callback, void* pUserData)
{
    CHECK_RAW(nHandle);
    ServerBase* obj = g_sdk->GetObj(nHandle);
    if (obj != NULL)
    {
        return obj->SetDataFlowCallback(callback, pUserData);
    }
    return -1;
}

BATNETSDK_API int BATNetSDKRaw_SetMsgCallBack(int nHandle, RAWMESSAGE_CALLBACK* callback, void* pUserData)
{
    CHECK_RAW(nHandle);
    ServerBase* obj = g_sdk->GetObj(nHandle);
    if (obj != NULL)
    {
        return obj->SetRawMsgCallback(callback, pUserData);
    }
    return -1;
}

BATNETSDK_API int BATNetSDKRaw_Send(int nHandle, int sessionId, const char* buf, int len)
{
    CHECK_RAW(nHandle);
    ServerBase* obj = g_sdk->GetObj(nHandle);
    if (obj != NULL)
    {
        return obj->Send(sessionId, buf, len);
    }
    return -1;
}

BATNETSDK_API int BATNetSDKRaw_SendAll(int nHandle, const char* buf, int len)
{
    CHECK_RAW(nHandle);
    ServerBase* obj = g_sdk->GetObj(nHandle);
    if (obj != NULL)
    {
        return obj->SendAll(buf, len);
    }
    return -1;
}

BATNETSDK_API int BATNetSDKRaw_Disconnect(int nHandle, int sessionId)
{
    CHECK_RAW(nHandle);
    ServerBase* obj = g_sdk->GetObj(nHandle);
    if (obj != NULL)
    {
        return obj->Disconnect(sessionId);
    }
    return -1;
}
