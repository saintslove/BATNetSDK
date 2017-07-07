// **************************************************************
// File        : BATNetSDKRawAPI.h
// Description : 北斗数据平台系统内部网络通讯库头文件
// Author      : LiuRuihan
// Date        : 2016-7-24
// Version     : V1.0.0.0
// Revisions   : 
// **************************************************************

#ifndef  BATNETSDKRAWAPI_H
#define BATNETSDKRAWAPI_H

#include "Common.h"
#include "CCMS.h"

#define BATNETSDK_API BATSDK_API

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************
*                               回调函数                               *
*********************************************************************/

/*
 函数: RAWMSG_CALLBACK
 功能: 消息回调
 参数:
     sessionId  会话id
     buf        消息数据
     len        消息数据长度
     userdata   用户私有参数
*/
typedef int (RAWMESSAGE_CALLBACK)(int sessionId, const char* buf, int len, void* userdata);

/*
 函数: CONNECT_CALLBACK
 功能: 连接状态回调
 参数:
     sessionId  会话id
     status     状态码 0-连接 1-断开
     userdata   用户私有参数
*/
typedef int (RAWCONNECT_CALLBACK)(int sessionId, int status, const char* ip, unsigned short port, void* userdata);

typedef int (RAWDATAFLOW_CALLBACK)(int sessionId, int recvbytes, void* userdata);

/***************************************************************
*                               导出接口                        *
***************************************************************/

/*
 函数: BATNetSDK_Init
 功能: 初始化
 参数:
     [in]nDevType   设备类型，参考enum emDevType
     [in]strDevSN   设备SN
     [in]bAutoLogin 自动登录标识 0:不自动登录 1:自动登录
 返回值: 成功0，否则返回失败原因
*/
#ifndef _BATNetSDK_Init_
#define _BATNetSDK_Init_
BATNETSDK_API int BATNetSDK_Init(int nDevType, char* strDevSN, bool bAutoLogin = true);
#endif

/*
 函数: BATNetSDK_Release
 功能: 释放资源
 参数: 无
 返回值: 无
*/
#ifndef _BATNetSDK_Release_
#define _BATNetSDK_Release_
BATNETSDK_API void BATNetSDK_Release();
#endif

/*
 函数: BATNetSDK_CreateClientObj
 功能: 创建一个客户端模式的对象
 参数:
     [in]nProtocolType  协议类型，参考enum emProtocolType
     [in]pServerAddr    要连接的服务器地址
 返回值: 成功返回创建的对象句柄，否则返回失败原因
*/
BATNETSDK_API int BATNetSDKRaw_CreateClientObj(CCMS_NETADDR* pServerAddr, int nProtocolType = 0);

/*
 函数: BATNetSDK_CreateServerObj
 功能: 创建一个服务端模式的对象
 参数:
     [in]nProtocolType  协议类型，参考enum emProtocolType
     [in]pListenAddr    要监听的本地设备的地址
 返回值: 成功返回创建的对象句柄，否则返回失败原因
*/
BATNETSDK_API int BATNetSDKRaw_CreateServerObj(CCMS_NETADDR* pListenAddr, int nProtocolType = 0);

/*
 函数: BATNetSDK_DeleteObj
 功能: 删除对象
 参数:
     [in]nHandle    要删除的对象句柄
 返回值: 成功0，否则返回失败原因
*/
BATNETSDK_API int BATNetSDKRaw_DeleteObj(int nHandle);

BATNETSDK_API int BATNetSDKRaw_Start(int nHandle);

BATNETSDK_API int BATNetSDKRaw_SetMsgCallBack(int nHandle, RAWMESSAGE_CALLBACK* callback, void* pUserData);

BATNETSDK_API int BATNetSDKRaw_SetConnCallBack(int nHandle, RAWCONNECT_CALLBACK* callback, void* pUserData);

BATNETSDK_API int BATNetSDKRaw_SetDataFlowCallBack(int nHandle, RAWDATAFLOW_CALLBACK* callback, void* pUserData);

BATNETSDK_API int BATNetSDKRaw_Send(int nHandle, int sessionId, const char* buf, int len);

BATNETSDK_API int BATNetSDKRaw_SendAll(int nHandle, const char* buf, int len);

BATNETSDK_API int BATNetSDKRaw_Disconnect(int nHandle, int sessionId);

#ifdef __cplusplus
}
#endif

#endif // BATNETSDKAPI_H
