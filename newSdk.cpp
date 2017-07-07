/*
 * newSdk.cpp
 *
 *  Created on: 2017年2月28日
 *      Author: wong
 */

#include "BATNetSDKAPI.h"

#include <stddef.h>

#include "SDKManager.h"
#include "CommonServer.h"
#include "sdk.h"

extern SDKManager* g_sdk;

#define bRaw false

BATNETSDK_API int BATNetSDK_Init(int nDevType, char* strDevSN, bool bAutoLogin/*=true*/)
{
    return Init(nDevType, strDevSN, bAutoLogin);
}

BATNETSDK_API void BATNetSDK_Release()
{
    Release();
}

BATNETSDK_API int BATNetSDK_CreateClientObj(CCMS_NETADDR* pServerAddr, int nProtocolType)
{
    return CreateClientObj(pServerAddr, nProtocolType, bRaw);
}

BATNETSDK_API int BATNetSDK_CreateServerObj(CCMS_NETADDR* pListenAddr, int nProtocolType)
{
    return CreateServerObj(pListenAddr, nProtocolType, bRaw);
}

BATNETSDK_API int BATNetSDK_DeleteObj(int nHandle)
{
    return DeleteObj(nHandle);
}

BATNETSDK_API int BATNetSDK_Start(int nHandle)
{
    CHECK_NEW(nHandle);
    CommonServer* obj = g_sdk->GetObj2(nHandle);
    if (obj != NULL)
    {
        return obj->Start();
    }
    return -1;
}

BATNETSDK_API int BATNetSDK_SetMsgCallBack(int nHandle,
    MESSAGE_CALLBACK *MsgProc, void* pUserData)
{
    CHECK_NEW(nHandle);
    CommonServer* obj = g_sdk->GetObj2(nHandle);
    if (obj != NULL)
    {
        return obj->SetMsgCallback(MsgProc, pUserData);
    }
    return -1;
}

BATNETSDK_API int BATNetSDK_SetConnCallBack(int nHandle,
    CONNECT_CALLBACK *ConnProc, void* pUserData)
{
    CHECK_NEW(nHandle);
    CommonServer* obj = g_sdk->GetObj2(nHandle);
    if (obj != NULL)
    {
        return obj->SetConnCallback(ConnProc, pUserData);
    }
    return -1;
}

BATNETSDK_API int BATNetSDK_SetDataFlowCallBack(int nHandle,
    DATAFLOW_CALLBACK *callback, void* pUserData)
{
    CHECK_NEW(nHandle);
    CommonServer* obj = g_sdk->GetObj2(nHandle);
    if (obj != NULL)
    {
        return obj->SetDataFlowCallback(callback, pUserData);
    }
    return -1;
}

BATNETSDK_API int BATNetSDK_SendData(int nHandle, int sessionId, int msgId, const char* buf, int len)
{
    CHECK_NEW(sessionId);
    CommonServer* obj = g_sdk->GetObj2(nHandle);
    if (obj != NULL)
    {
        return obj->SendData(sessionId, msgId, buf, len);
    }
    return -1;
}

BATNETSDK_API int BATNetSDK_SendCommon(int nHandle, int sessionId, int msgId, int result)
{
    CHECK_NEW(nHandle);
    CommonServer* obj = g_sdk->GetObj2(nHandle);
    if (obj != NULL)
    {
        return obj->SendCommon(sessionId, msgId, result);
    }
    return -1;
}

BATNETSDK_API int BATNetSDK_SendBySN(int nHandle, const char* strDevSN, int msgId, const char* buf, int len)
{
    CHECK_NEW(nHandle);
    CommonServer* obj = g_sdk->GetObj2(nHandle);
    if (obj != NULL)
    {
        return obj->SendData(strDevSN, msgId, buf, len);
    }
    return -1;
}

BATNETSDK_API int BATNetSDK_SendByDevType(int nHandle, int nDevType, int msgId, const char* buf, int len)
{
    CHECK_NEW(nHandle);
    CommonServer* obj = g_sdk->GetObj2(nHandle);
    if (obj != NULL)
    {
        return obj->SendData((emDevType)nDevType, msgId, buf, len);
    }
    return -1;
}
