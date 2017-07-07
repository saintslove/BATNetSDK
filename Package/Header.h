/*
 * Header.h
 *
 *  Created on: 2016年6月29日
 *      Author: wong
 */

#ifndef PACKAGE_HEADER_H_
#define PACKAGE_HEADER_H_

#include "../common.h"

#include <inttypes.h>
#include <string>

#define PACKAGE_MAGIC_NUMBER ('@' << 24 | 'B' << 16 | 'A' << 8 | 'T')
#define PACKAGE_MAX_LENGTH 65535 // 最大包长度
#define PACKAGE_CUR_VERSION 100 // 当前版本号

enum DeviceType
{
    DT_Unknown          = 0,
    DT_CenterServer     = 1000, // 中心服务器
    DT_808Adapter       = 1001, // 808接入服务器
    DT_809Adapter       = 1002, // 809接入服务器
    DT_AnalysisServer   = 1003, // 分析服务器
    DT_MediaServer      = 1004, // 流媒体服务器
    DT_BusinessServer   = 1005, // 业务接口服务器
    DT_StorageServer    = 1006, // 存储服务器
    DT_DBServer         = 1007, // 数据库服务器
    DT_SupervisionServer= 1008, // 上级监管中心服务器
    DT_FEPTicketServer  = 1009, // 票务数据前置机
    DT_MDVR             = 1010, // 车载终端机
    DT_ExternalReceiver = 1011, // 外部数据接收服务器
    DT_PushServer       = 1012, // 数据转发服务器
    DT_GPSAdapter       = 1013, // GPS数据接入服务器
    DT_TicketAdapter    = 1014, // 票务数据接入服务器
};

enum MessageId
{
    MSG_Unknown             = 0,
    // 公用协议
    MSG_Login               = 0x1001, // 登录
    MSG_LoginResp           = 0x9101, // 登录应答
    MSG_Logout              = 0x1002, // 注销
    MSG_LogoutResp          = 0x9102, // 注销应答
    MSG_KeepAlive           = 0x1003, // 心跳
    MSG_FEPCommonResp       = 0x0001, // 前置机通用应答
    MSG_PlatformCommonResp  = 0x9001, // 平台通用应答
    // 位置数据接入协议
    MSG_Position            = 0x2001,
    MSG_Alarm               = 0x2002,
    // 票务数据接入协议
    MSG_TicketBaseSchedule  = 0x3001, // 班次信息
    MSG_TicketSchedule      = 0x3002, // 发班班次信息
    MSG_TicketSendSchedule  = 0x3003, // 发班信息
    MSG_TicketOrderDetail   = 0x3004, // 订单明细
};

/*class Header
{
public:
    Header();
    virtual ~Header();

public:
    bool Valid();
    size_t GetHeaderLen();
    int Encode(char* buf, const size_t len);
    int Decode(const char* buf, const size_t len);
    std::string ToString();

private:
    bool CheckParam(const char* buf, const size_t len);

public:
    uint32_t u4Head;
    uint32_t u4Version;
    uint16_t u2DeviceType;
    uint32_t u4SeqNo;
    uint32_t u4AckNo;
    uint32_t u4MessageId;
    uint32_t u4SessionId;
    uint32_t u4BodyLen;
    uint64_t u8Timestamp;
    uint32_t u4Reserved;
};*/

#pragma pack(1)
struct Header
{
    uint32_t u4Head;
    uint32_t u4Version;
    uint16_t u2DeviceType;
    uint32_t u4SeqNo;
    uint32_t u4AckNo;
    uint32_t u4MessageId;
    uint32_t u4SessionId;
    uint32_t u4BodyLen;
    uint64_t u8Timestamp;
    uint32_t u4Reserved;
};
#pragma pack()

class Package
{
public:
    Package();
    Package(MessageId msgId, const std::string& name);
    virtual ~Package();

public:
    static size_t GetHeaderLen();
    static int PeekPackage(const char* buf, const size_t len, uint32_t& msgId);

public:
    bool Valid();
    size_t GetLength();
    int Encode(char* buf, const size_t len);
    int Decode(const char* buf, const size_t len);
    std::string GetName();
    std::string ToString();
    std::string HeaderToString();

protected:
    virtual size_t GetBodyLen() {return 0;}
    virtual int EncodeBody(char** buf, const size_t len) {return EC_Ok;}
    virtual int DecodeBody(const char** buf, const size_t len) {return EC_Ok;}
    virtual std::string BodyToString() {return "";}

private:
    void Init();
    bool ValidHeader();
    int EncodeHeader(char** buf, const size_t len);
    int DecodeHeader(const char** buf, const size_t len);

public:
    Header header;

protected:
    std::string name;
};

#endif /* PACKAGE_HEADER_H_ */
