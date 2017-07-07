// **************************************************************
// File        : BATNetSDKAPI.h
// Description : 北斗数据平台系统内部网络通讯库头文件
// Author      : LiuRuihan
// Date        : 2016-7-24
// Version     : V1.0.0.0
// Revisions   : 
// **************************************************************

#ifndef  BATNETSDKAPI_H
#define BATNETSDKAPI_H

#include "Common.h"
#include "CCMS.h"

#define BATNETSDK_API BATSDK_API

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************
*                               回调函数                        *
***************************************************************/

/*
 函数: MESSAGE_CALLBACK
 功能: 消息回调
 参数:
     msgId      消息ID
     buf        消息数据
     len        消息数据长度
     userdata   用户私有参数
*/
typedef int (MESSAGE_CALLBACK)(int sessionId, int msgId, const char* buf, int len, void* userdata);

/*
 函数: CONNECT_CALLBACK
 功能: 连接状态回调
 参数:
     sn          编号
     status      状态码 0-连接 1-断开 2-登录 3-登出 4-超时断开
     userdata    用户私有参数
*/
typedef int (CONNECT_CALLBACK)(int sessionId, const char* sn, int status, void* userdata);

typedef int (DATAFLOW_CALLBACK)(int sessionId, const char* sn, int recvbytes, void* userdata);

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
BATNETSDK_API int BATNetSDK_CreateClientObj(CCMS_NETADDR* pServerAddr, int nProtocolType = 0);

/*
 函数: BATNetSDK_CreateServerObj
 功能: 创建一个服务端模式的对象
 参数:
     [in]nProtocolType  协议类型，参考enum emProtocolType
     [in]pListenAddr    要监听的本地设备的地址

 返回值: 成功返回创建的对象句柄，否则返回失败原因
*/
BATNETSDK_API int BATNetSDK_CreateServerObj(CCMS_NETADDR* pListenAddr, int nProtocolType = 0);

/*
 函数: BATNetSDK_DeleteObj
 功能: 删除对象
 参数:
     [in]nHandle    要删除的对象句柄
 返回值: 成功0，否则返回失败原因
*/
BATNETSDK_API int BATNetSDK_DeleteObj(int nHandle);

BATNETSDK_API int BATNetSDK_Start(int nHandle);

BATNETSDK_API int BATNetSDK_SetMsgCallBack(int nHandle, MESSAGE_CALLBACK* callback, void* pUserData);

BATNETSDK_API int BATNetSDK_SetConnCallBack(int nHandle, CONNECT_CALLBACK* callback, void* pUserData);

BATNETSDK_API int BATNetSDK_SetDataFlowCallBack(int nHandle, DATAFLOW_CALLBACK* callback, void* pUserData);

BATNETSDK_API int BATNetSDK_SendData(int nHandle, int sessionId, int msgId, const char* buf, int len);

BATNETSDK_API int BATNetSDK_SendCommon(int nHandle, int sessionId, int msgId, int result);

BATNETSDK_API int BATNetSDK_SendBySN(int nHandle, const char* strDevSN, int msgId, const char* buf, int len);

BATNETSDK_API int BATNetSDK_SendByDevType(int nHandle, int nDevType, int msgId, const char* buf, int len);

#ifdef __cplusplus
}
#endif

#endif // BATNETSDKAPI_H
