/*
 * common.h
 *
 *  Created on: 2016年7月14日
 *      Author: wong
 */

#ifndef COMMON_H_
#define COMMON_H_

enum ErrorCode
{
    EC_Ok               = 0, // 成功
    EC_Fail             = 1 << 1, // 失败
    EC_Continue         = 1 << 2, // 操作未完成,等待继续
    EC_InvalidParam     = 1 << 3, // 参数不合法
    EC_BufferNotEnough  = 1 << 4, // 缓冲区长度不够
    EC_InvalidState     = 1 << 5, // 状态不合法
};

enum ConnStatus
{
    CS_Connected = 0,   // 连接
    CS_Disconnect = 1,  // 断开
    CS_Login = 2,       // 登录
    CS_Logout = 3,      // 登出
    CS_Timeout = 4,     // 超时断开
    CS_TimeoutOnce = 5  // 单次超时
};

#endif /* COMMON_H_ */
