// **************************************************************
// File        : Ccms.h
// Description : 北斗数据平台系统内部通讯协议头文件
// Author      : LiuRuihan
// Date        : 2016-7-24
// Version     : V1.0.0.0
// Revisions   : 
// **************************************************************

#ifndef _CCMS_H_
#define _CCMS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************************
*
*                               数据类型定义
*
********************************************************************************************/
typedef signed long         PointerSizedInt;
typedef unsigned long       PointerSizedUInt;
typedef unsigned char       UInt8;
typedef signed char         SInt8;
typedef unsigned short      UInt16;
typedef signed short        SInt16;
typedef unsigned int        UInt32;
typedef signed int          SInt32;
typedef signed long long    SInt64;
typedef unsigned long long 	UInt64;
typedef float               Float32;
typedef double              Float64;
typedef UInt16              Bool16;
typedef UInt8               Bool8;


/*******************************************************************************************
*
*                               错误码定义
*
********************************************************************************************/
#define CCMS_RETURN_OK              0   // 成功
#define CCMS_ERRORMSG_FAIL          -1  // 失败
#define CCMS_ERRORMSG_DEVTYPE       -2  // 设备类型错误
#define CCMS_ERRORMSG_DBQUERY       -3  // 数据库查询失败
#define CCMS_ERRORMSG_AUTH          -4  // 登录失败，错误的用户名或密码
#define CCMS_ERRORMSG_ALREADYLOGGED -5  // 登录失败，用户已经登录。
#define CCMS_ERRORMSG_BUSY          -6  // 服务器忙
#define CCMS_ERRORMSG_GETURL        -7  // 获取视频浏览URL失败
#define CCMS_ERRORMSG_OPENSTREAM    -8  // 打开视频流失败
#define CCMS_ERRORMSG_CLOSESTREAM   -9  // 关闭视频流失败
#define CCMS_ERRORMSG_IP            -10 // IP地址错误
#define CCMS_ERRORMSG_CONNECTSERVER -11 // 连接服务器失败
#define CCMS_ERRORMSG_SENDDATA      -12 // 发送网络数据失败
#define CCMS_ERRORMSG_RECVDATA      -13 // 接收网络数据失败
#define CCMS_ERRORMSG_INVALIDPARAM  -14 // 参数错误
#define CCMS_RETURN_CONTINUE        -15 // 操作未完成, 继续等待

/*******************************************************************************************
*
*                               通信消息定义
*
********************************************************************************************/
//通用部分
#define CCMS_DEV_COMMONRESPMSG      0x0001  // 设备通用应答
#define CCMS_PLATFORM_COMMONRESPMSG 0x9001  // 平台通用应答
#define CCMS_LOGIN_MSG              0x1001  // 登录
#define CCMS_LOGIN_RESPMSG          0x9101  // 登录响应消息
#define CCMS_LOGOUT_MSG             0x1002  // 注销
#define CCMS_LOGOUT_RESPMSG         0x9102  // 注销响应消息
#define CCMS_KEEPALIVE_MSG          0x1003  // 心跳保持消息

#define CCMS_POSITIONDATA_MSG       0x2001  // 发送位置数据消息
#define CCMS_ALARMDATA_MSG          0x2002  // 发送报警数据消息

//票务数据
#define CCMS_BASESCHEDULEDATA_MSG           0x3001  // 发送班次信息消息
#define CCMS_BASESCHEDULE_DETAILDATA_MSG    0x3002  // 发送班次车辆信息消息
#define CCMS_BASESCHEDULE_PRICEDATA_MSG     0x3003  // 发送班次票价信息消息
#define CCMS_SCHEDULEDATA_MSG               0x3004  // 发送发班班次信息消息
#define CCMS_SCHEDULE_DETAILDATA_MSG        0x3005  // 发送发班班次票价信息消息
#define CCMS_SENDSCHEDULEDATA_MSG           0x3006  // 发送发班信息消息
#define CCMS_SENDSCHEDULE_DETAILDATA_MSG    0x3007  // 发送发班信息到站明细消息
#define CCMS_ORDERUPLOADDATA_MSG            0x3008  // 发送订单明细消息
#define CCMS_ORDERUPLOAD_TICKETDATA_MSG     0x3009  // 发送订单明细上报票价消息

//票务回包
#define CCMS_BASESCHEDULEDATA_RESPMSG           0x3101  // 发送班次回包消息
#define CCMS_BASESCHEDULE_DETAILDATA_RESPMSG    0x3102  // 发送班次车辆回包消息
#define CCMS_BASESCHEDULE_PRICEDATA_RESPMSG     0x3103  // 发送班次票价回包消息
#define CCMS_SCHEDULEDATA_RESPMSG               0x3104  // 发送发班班次回包消息
#define CCMS_SCHEDULE_DETAILDATA_RESPMSG        0x3105  // 发送发班班次票价回包消息
#define CCMS_SENDSCHEDULEDATA_RESPMSG           0x3106  // 发送发班回包消息
#define CCMS_SENDSCHEDULE_DETAILDATA_RESPMSG    0x3107  // 发送发班信息到站明细回包消息
#define CCMS_ORDERUPLOADDATA_RESPMSG            0x3108  // 发送订单明细回包消息
#define	CCMS_ORDERUPLOAD_TICKETDATA_RESPMSG     0x3109  // 发送订单明细上报票价回包消息

//深圳公交数据
#define CCMS_SZBUSGPS_MSG                       0x4000


/*******************************************************************************************
*
*                               数据结构定义
*
********************************************************************************************/
#pragma pack (1)
typedef struct T_CCMS_NETADDR
{
    UInt8  chIP[16];    // IPv4地址，点分十进制格式
    UInt16 nPort;       // 端口号
} CCMS_NETADDR;

typedef struct T_DATETIME
{
    UInt16 nYear;
    UInt8 nMonth;
    UInt8 nDay;
    UInt8 nHour;
    UInt8 nMinute;
    UInt8 nSecond;
} DATETIME;


//共同包头，所有网络信令的起始位置都必须附带
typedef struct T_CCMS_MSG_HEAD
{
    UInt32 nHead;       // 统一设置为@BAT
    UInt32 nVersion;    // 版本号
    UInt16 nDevtype;    // 设备类型,参考emDevType
    UInt32 nSeqid;      // 网络消息传递序列号
    UInt32 nRseqid;     // 网络消息回传序列号
    UInt32 nMsgid;      // 消息id
    UInt32 nSessionid;  // 会话id
    UInt32 nLength;     // 数据长度
    UInt64 nTimeStamp;  // 时间
    UInt32 nReserve;    // 保留字段
} CCMS_MSG_HEAD;

//位置信息
typedef struct T_CCMS_POSITION_DATA
{
    UInt8 chVehicleID[16];      // 车牌号
    UInt8 chSimCard[16];        // SIM卡号或IMSI码
    UInt32 nDeviceID;           // 设备号
    UInt32 nLatitude;           // 纬度
    UInt32 nLongitude;          // 经度
    UInt16 nHeight;             // 高度
    UInt16 nSpeed;              // 速度
    UInt16 nDirect;             // 方向
    UInt8 nStatus;              // 状态
    UInt8 nAlarmType;           // 报警类型，参考emAlarmType
    UInt8 nVehicleColor;        // 车牌颜色，参考emColorType 0：黄 1：黑  2：蓝  3：白
    DATETIME stDateTime;        // 时间
} CCMS_POSITION_DATA;


// 班次信息 0x3001
typedef struct T_CCMS_TICKET_BASESCHEDULE
{
    UInt8 chFServerSN[12];          // 票务前置机编号
    UInt8 chGuid[40];               // ID
    UInt8 chGlobalCode[10];         // 班次ID
    UInt8 chStationCode[8];         // 报送站代码
    UInt8 chStationName[80];        // 报送站名称
    UInt8 chLocalCode[20];          // 站务系统班次号
    UInt8 chLineName[80];           // 线路名称
    UInt8 chTransType[10];          // 客运类别
    UInt8 chLineKind[20];           // 线路种类
    UInt8 chSchKind[20];            // 班次种类
    UInt8 chSchType[20];            // 班次类型
    UInt8 chSchMode[10];            // 发班模式
    UInt8 chStartTime[4];           // 首班时间 HHmm
    UInt8 chEndTime[4];             // 末班时间 HHmm
    UInt16 nRotationInterval;       // 发班间隔时间
    UInt8 chTime[4];                // 发班时间 HHmm
    UInt8 chEndProvince[90];        // 终到省
    UInt8 chEndCity[90];            // 终到市
    UInt8 chEndCounty[20];          // 终到县
    UInt8 chEndTown[20];            // 终到镇
    UInt8 chVillage[20];            // 终到村
    UInt8 chEndNodeCode[20];        // 终到站代码
    UInt8 chEndNodeName[50];        // 终到站
    UInt8 chStartValidDate[8];      // 班次有效期起,格式为yyyyMMdd
    UInt8 chEndValidDate[8];        // 班次有效期止,格式为yyyyMMdd
    UInt8 chStatus[10];             // 班次状态
    DATETIME dtUploadTime;          // 上报时间
    DATETIME dtCreateTime;          // 创建时间
    DATETIME dtUpdateTime;          // 更新时间
} CCMS_TICKET_BASESCHEDULE;

//班次信息--车辆信息 0x3002
typedef struct T_CCMS_TICKET_BASESCHEDULE_DETAIL  
{
    UInt8 chFServerSN[12];          // 票务前置机编号
    UInt8 chGuid[40];               // Guid 作为副表的唯一标识
    UInt8 chParentID[40];           // 班次信息ID 跟T_CCMS_TICKET_BASESCHEDULE的chGuid关联
    UInt8 chStationCode[8];         // 客运站编码
    UInt8 chLocalCode[20];          // 站务系统班次号
    UInt8 chEnterpriseCode[15];     // 企业代码
    UInt8 chEnterpriseName[80];     // 企业名称
    UInt8 chBusCode[50];            // 车牌号
    UInt8 nSeatCount;               // 核定座位数
    UInt8 chBusinessBus;            // 是否商务大巴 1:是 0:否
    UInt8 chBusType[10];            // 车辆类型
    UInt8 chSeatType[10];           // 座型
    UInt8 chBusLevel[40];           // 车辆等级
    DATETIME dtCreateTime;          // 车辆创建时间
    DATETIME dtUpdateTime;          // 车辆更新时间
}CCMS_TICKET_BASESCHEDULE_DETAIL;

//班次信息--票价信息 0x3003
typedef struct T_CCMS_TICKET_BASESCHEDULEPRICE  
{
    UInt8 chFServerSN[12];          // 票务前置机编号
    UInt8 chGuid[40];               // Guid 作为副表的唯一标识
    UInt8 chParentID[40];           // 班次信息ID 跟T_CCMS_TICKET_BASESCHEDULE的chGuid关联
    UInt8 chStationCode[8];         // 站场编码
    UInt8 chLocalCode[20];          // 站务系统班次号
    UInt8 chNodeName[100];          // 中途站点简称
    UInt8 chNodeCode[10];          	// 中途站点代码
    UInt8 chTravelTime[4];          // 行驶时间 HHmm
    Float64 flMileage;              // 行驶里程
    Float64 flTransPrice;           // 运价
    Float64 flSingle;               // 单车单程车辆通行费
    Float64 flSingleTripToll;       // 单座车辆通行费
    Float64 flBunkerSurcharge;      // 燃油附加费
    Float64 flStationFee;           // 站务费
    Float64 flOtherFee;             // 其它费用金额
    Float64 flTransFee;             // 运费
    Float64 flUpperPrice;           // 全票上限价
    Float64 flPrice;                // 全票执行价
    Float64 flHalfprice;            // 半票执行价
    Float64 flStdPrice;             // 学生票价
    Float64 flDisabilitySoldierPrice;   // 残疾军人票价
    UInt8 chValidityStart[8];       // 执行有效期起
    UInt8 chValidityEnd[8];         // 执行有效期止
    DATETIME dtCreateTime;          // 创建时间
    DATETIME dtUpdateTime;          // 更新时间
    // UInt8 chMemo[255];           // 备注
}CCMS_TICKET_BASESCHEDULEPRICE;


//发班班次 0x3004
typedef struct T_CCMS_TICKET_SCHEDULE
{
    UInt8 chFServerSN[12];          // 票务前置机编号
    UInt8 chGuid[40];               // ID
    UInt8 chGlobalCode[10];         // 班次ID
    UInt8 chStationCode[8];         // 报送站代码
    UInt8 chStationName[80];        // 报送站名称
    UInt8 chLocalCode[20];          // 站务系统班次号
    UInt8 chLineName[80];           // 线路名称
    UInt8 chTransType[10];          // 客运类别
    UInt8 chLineKind[20];           // 线路种类
    UInt8 chSchKind[20];            // 班次种类
    UInt8 chSchType[20];            // 班次类型
    UInt8 chSchMode[10];            // 发班模式
    UInt8 chStartTime[4];           // 首班时间 HHmm
    UInt8 chEndTime[4];             // 末班时间 HHmm
    UInt16 nRotationInterval;       // 发班间隔时间
    UInt8 chTime[4];                // 发班时间 HHmm
    UInt8 chDate[8];                // 发班日期 yyyyMMdd
    UInt8 chEndProvince[90];        // 终到省
    UInt8 chEndCity[90];            // 终到市
    UInt8 chEndCounty[50];          // 终到县
    UInt8 chEndTown[50];            // 终到镇
    UInt8 chVillage[50];            // 终到村
    UInt8 chEndNodeCode[20];        // 终到站代码
    UInt8 chEndNodeName[50];        // 终到站
    UInt8 chStatus[20];             // 班次状态
    UInt16 nSeatCount;              // 总票数
    UInt16 nTicketSale;             // 已售票总数
    UInt16 nTicketSell;             // 剩余票总数
    UInt16 nTicketCount;            // 可售票总数
    UInt16 nFreeChildrenTicketSale; // 可售免票儿童数
    UInt8 chEnterpriseCode[15];     // 企业代码
    UInt8 chEnterpriseName[80];     // 企业名称
    UInt8 chBusCode[50];            // 车牌号
    UInt8 bBusinessBus;             // 是否商务大巴 1:是 0:否
    UInt8 chBusType[10];            // 车辆类型
    UInt8 chSeatType[10];           // 座型
    UInt8 chBusLevel[40];           // 车辆等级
    DATETIME dtUploadTime;          // 上报时间
    DATETIME dtUpdateTime;          // 更新时间
} CCMS_TICKET_SCHEDULE;

//发班班次--票价信息明细 0x3005
typedef struct T_CCMS_TICKET_SCHEDULEDETAIL  
{
    UInt8 chFServerSN[12];          // 票务前置机编号
    UInt8 chGuid[40];               // Guid 作为副表的唯一标识
    UInt8 chParentID[40];           // 发班班次ID 与T_CCMS_TICKET_SCHEDULE的chGuid关联
    UInt8 chStationCode[8];         // 始发站代码
    UInt8 chLocalCode[20];          // 站务系统班次号
    UInt8 chDate[8];                // 发班日期 yyyyMMdd
    UInt8 chNodeCode[10];           // 中途站代码
    UInt8 chNodeName[100];          // 中途站名称
    Float64 flTravelTime;           // 行驶时间
    Float64 flMileage;              // 行驶里程
    Float64 flUpperPrice;           // 全票上限价
    Float64 flPrice;                // 全票执行价
    Float64 flHalfprice;            // 半票执行价
    Float64 flStdPrice;             // 学生票价
    Float64 flDisabilitySoldierPrice;   // 残疾军人票价
    UInt16 nFullTicketSale;         // 已售全票数
    UInt16 nHalfTicketSale;         // 已售半票数
    UInt16 nFreeTicketSale;         // 免票数
    UInt16 nStdTicketSale;          // 已售学生票数
    UInt16 nDisabilitySoldierTicketSell; // 已售残疾军人票数
    DATETIME dtCreateTime;          // 创建时间
    DATETIME dtUpdateTime;          // 更新时间
} CCMS_TICKET_SCHEDULEDETAIL;


//发班信息 0x3006
typedef struct T_CCMS_TICKET_SENDSCHEDULE
{
    UInt8 chFServerSN[12];          // 票务前置机编号
    UInt8 chGuid[40];               // ID
    UInt8 chGlobalCode[10];         // 班次ID
    UInt8 chStationCode[8];         // 报送站代码
    UInt8 chStationName[80];        // 报送站名称
    UInt8 chBusIC[20];              // 车辆IC卡号
    UInt8 chBusCode[20];            // 车牌号
    UInt8 chBusColor[10];           // 车牌颜色
    UInt8 chEnterpriseCode[15];     // 企业代码
    UInt8 chEnterpriseName[80];     // 企业名称
    UInt8 chLocalCode[20];          // 站务系统班次号
    UInt8 chDate[8];                // 发班日期 yyyyMMdd
    UInt8 chTime[4];                // 发班时间 HHmm
    UInt8 chInTime[4];              // 进站时间 HHmm
    UInt8 chOutTime[4];             // 出站时间 HHmm
    UInt8 chSafeCheck[6];           // 安检结果
    UInt16 nPassenger;              // 本站上客人数
    UInt16 nPassengerIn;            // 进站时总人数
    UInt16 nPassengerFee;           // 免票人数
    UInt16 nChild;                  // 免票儿童人数
    UInt16 nPassengerOut;           // 出站时总人数
    //UInt32 nPassengerNet;         // 省联网售票人数
    UInt8 chOper[20];               // 记录人
    DATETIME dtOpTime;              // 记录时间
    DATETIME dtUploadTime;          // 上报时间
    UInt8 chIsSafeSuper[50];        // 安全监管报送标记
    UInt8 chRoadTransCode[40];      // 道路运输证号
    UInt8 chSeatType[10];           // 座位
    UInt8 chBusLevel[40];           // 车辆等级
    UInt8 chTechLevel[40];          // 技术等级
    UInt8 chCarValid[8];            // 审验有效期,格式为yyyyMMdd
    UInt8 chMarkCarCode[40];        // 标志牌号码
    UInt8 chLineName[80];           // 线路名称
    UInt8 chThroughLine[255];       // 途径路线
    UInt8 chNodes[255];             // 配客地点
    UInt8 chEndNodeCode[20];        // 终到站代码
    UInt8 chEndNodeName[50];        // 终到站名称
    UInt8 chSchKind[20];            // 班次种类
    UInt8 chSchType[20];            // 班次类型
    UInt8 chSchMode[10];            // 发班模式
    UInt8 chDrivingLicenseValid[8]; // 行驶证有效性
    UInt8 bIsDanger;                // 是否配载危险物品 1:是 0:否
    UInt8 chDangerName[255];        // 危险物品名称
    UInt8 bIsSafetyBelt;            // 是否系好安全带 1:是 0:否
    UInt8 bIsOverman;               // 是否超员 1:是 0:否
    UInt8 chDriver1[20];            // 驾驶员1姓名
    UInt8 chDriverCer1[20];         // 驾驶员1从业资格证号
    UInt8 bDriverValid1;            // 驾驶员1驾驶证有效性 1:是 0:否
    UInt8 chDriver2[20];            // 驾驶员2姓名
    UInt8 chDriverCer2[20];         // 驾驶员2从业资格证号
    UInt8 bDriverValid2;            // 驾驶员2驾驶证有效性 1:是 0:否
    UInt8 chDriver3[20];            // 驾驶员3姓名
    UInt8 chDriverCer3[20];         // 驾驶员3从业资格证号
    UInt8 bDriverValid3;            // 驾驶员3驾驶证有效性 1:是 0:否
    DATETIME dtCreateTime;          // 创建时间
    DATETIME dtUpdateTime;          // 更新时间
} CCMS_TICKET_SENDSCHEDULE;

//发班信息--到站明细 0x3007
typedef struct T_CCMS_TICKET_SENDSCHEDULEDETAIL		
{
    UInt8 chFServerSN[12];          // 票务前置机编号
    UInt8 chGuid[40];               // Guid 作为副表的唯一标识
    UInt8 chParentID[40];           // 发班信息主表ID 与T_CCMS_TICKET_SENDSCHEDULE的 chGuid关联
    UInt8 chStationCode[8];         // 发站编码
    UInt8 chDate[8];                // 发班日期 yyyyMMdd
    UInt8 chLocalCode[20];          // 站务系统班次号
    UInt8 chNodeCode[20];           // 到站编码
    UInt8 chNodeName[50];           // 到站名称
    UInt16 nNodeNumber;             // 到站人数
    DATETIME dtCreateTime;          // 创建时间
    DATETIME dtUpdateTime;          // 更新时间
} CCMS_TICKET_SENDSCHEDULEDETAIL;

// 订单信息 0x3008
typedef struct T_CCMS_TICKET_ORDERUPLOAD
{
    UInt8 chFServerSN[12];          // 票务前置机编号
    UInt8 chGuid[40];               // ID
    UInt8 chAgentCode[20];          // 操作方
    UInt8 chStationCode[10];        // 客运站代码 **注意此处与其他地方不一样,未取票之前为10位代码
    UInt8 chStationName[80];        // 客运站名称
    UInt8 chOrderNo[30];            // 订单号
    UInt8 chScheduleNo[20];         // 班次号
    UInt16 nTicketCount;            // 票数
    Float64 flTicketAmount;         // 交易金额
    Float64 flAgentFee;             // 代理手续费
    Float64 flRefundFee;            // 退票手续费
    UInt8 chBarCodes[255];          // 条码号 以半角,分割
    UInt8 chTicketNos[255];         // 票号 以半角,分割
    UInt16 nRequestType;            // 请求类型 0-成功订单；1-退票订单；2-废票订单
    UInt8 chTransStatus[15];        // 交易状态
    UInt16 nUploadMemberType;       // 上报机构类型 1-分销机构；2-客运站
    DATETIME dtOrderTime;           // 提交时间
    UInt8 chOperCode[20];           // 操作员工号
    UInt8 chOperName[50];           // 操作时间
    UInt8 chOpAddress[255];         // 操作地址
    UInt8 chTransID[40];            // 事务ID
    UInt8 chPaySn[40];              // 支付单号
    UInt8 chPayType[40];            // 支付类型
    UInt8 chRemark[255];            // 备注
    UInt8 chBllType[5];             // 业务类型
    DATETIME dtCreateTime;          // 创建时间
    DATETIME dtUpdateTime;          // 更新时间
} CCMS_TICKET_ORDERUPLOAD;

//订单票明细 0x3009
typedef struct T_CCMS_TICKET_ORDERUPLOADTICKET
{
    UInt8 chFServerSN[12];      // 票务前置机编号
    UInt8 chGuid[40];           // Guid 作为副表的唯一标识
    UInt8 chParentID[40];       // 订单ID 与T_CCMS_TICKET_ORDERUPLOAD的 chGuid关联
    UInt8 chBarCode[10];        // 始发站代码 **注意此处与其他地方位数不一样
    UInt8 chTicketNo[10];       // 票号
    UInt8 chOrderNo[30];        // 订单号
    DATETIME ullCreateTime;     // 创建时间
    DATETIME ullUpdateTime;     // 更新时间
} CCMS_TICKET_ORDERUPLOADTICKET;

// 票务回包 0x3101-0x3109 对应0x3001-0x3009
typedef struct T_CCMS_TICKET_RESP
{
    UInt8 chFServerSN[12];  // 票务前置机编号
    UInt8 chGuid[40];       // Guid 作为副表的唯一标识
} CCMS_TICKET_RESP;
#pragma pack ()


//设备类型
typedef enum emDevTypeTag
{
    CCMS_DEVTYPE_UNKNOWN = 0,
    CCMS_DEVTYPE_CMSERVER = 1000,   // 中心管理服务器
    CCMS_DEVTYPE_ADAPTER808,        // 808接入服务器
    CCMS_DEVTYPE_ADAPTER809,        // 809接入服务器
    CCMS_DEVTYPE_ANALYSIS,          // 分析服务器
    CCMS_DEVTYPE_STREAMSERVER,      // 流媒体服务器
    CCMS_DEVTYPE_WEBSERVER,         // 业务接口服务器
    CCMS_DEVTYPE_NVR,               // 存储服务器
    CCMS_DEVTYPE_DBSERVER,          // 数据库服务器
    CCMS_DEVTYPE_MONITORCENTER,     // 上级监管中心
    CCMS_DEVTYPE_PROCESSOR,         // 前置机
    CCMS_DEVTYPE_MDVR,              // 车载GPS终端
    CCMS_DEVTYPE_FRECVSERVER,       // 外部的数据接收服务器
    CCMS_DEVTYPE_DELIVER            // 转发服务器
} emDevType;

//协议类型
typedef enum emProtocolTypeTag
{
    CCMS_PROTOCOLTYPE_808 = 5001,   // 北斗终端部标协议808
    CCMS_PROTOCOLTYPE_809,          // 北斗部标平台协议809
    CCMS_PROTOCOLTYPE_SZTBGPS,      // 深圳交委GPS协议
    CCMS_PROTOCOLTYPE_SZTBTICKET    // 深圳交委票务协议
} emProtocolType;

//报警类型
typedef enum emAlarmTypeTag
{
    CCMS_ALARMTYPE_EMERGENCY = 1,       // 紧急报警
    CCMS_ALARMTYPE_OVERSPEED,           // 超速报警
    CCMS_ALARMTYPE_TIREDDRIVING,        // 疲劳驾驶报警
    CCMS_ALARMTYPE_DANGERPREDICT,       // 危险预警
    CCMS_ALARMTYPE_GNSSFAULT,           // GNSS模块发生故障
    CCMS_ALARMTYPE_GNSSLINEOPEN,        // GNSS天线未接或被剪断
    CCMS_ALARMTYPE_GNSSLINESHORT,       // GNSS天线短路
    CCMS_ALARMTYPE_MDTPOWERLOWVALTAGE,  // 终端主电源欠压
    CCMS_ALARMTYPE_MDTPOWEROFF,         // 终端主电源掉电
    CCMS_ALARMTYPE_MDTLCDFAULT,         // 终端LCD或显示器故障
    CCMS_ALARMTYPE_TTSFAULT,            // TTS模块故障
    CCMS_ALARMTYPE_CAMERAFAULT,         // 摄像头故障
    CCMS_ALARMTYPE_ICCARDFAULT,         // 道路运输证IC卡模块故障
    CCMS_ALARMTYPE_OVERSPEEDPREDICT,    // 超速预警
    CCMS_ALARMTYPE_TIREDDRIVINGPREDICT, // 疲劳驾驶预警
    CCMS_ALARMTYPE_DRIVINGOVERTIME,     // 当天累计驾驶超时
    CCMS_ALARMTYPE_STOPOVERTIME,        // 超时停车
    CCMS_ALARMTYPE_AREAINOUT,           // 进出区域
    CCMS_ALARMTYPE_AREAIN,              // 进区域
    CCMS_ALARMTYPE_AREAOUT,             // 出区域
    CCMS_ALARMTYPE_ROUTEINOUT,          // 进出路线
    CCMS_ALARMTYPE_ROUTEDRIVINGTIME,    // 路段行驶时间不足/过长
    CCMS_ALARMTYPE_ROUTEDEVIATE,        // 线路偏离报警
    CCMS_ALARMTYPE_VSSFAULT,            // 车辆VSS故障
    CCMS_ALARMTYPE_OILEXCEPTIION,       // 车辆油量异常
    CCMS_ALARMTYPE_BESTOLEN,            // 车辆被盗
    CCMS_ALARMTYPE_ILLEGALON,           // 车辆非法点火
    CCMS_ALARMTYPE_ILLEGALMOVE,         // 车辆非法位移
    CCMS_ALARMTYPE_ACROSSPREDICT,       // 碰撞预警
    CCMS_ALARMTYPE_ROLLOVERPREDICT,     // 侧翻预警
    CCMS_ALARMTYPE_ILLEGALOPENDOOR,     // 非法开门报警
    CCMS_ALARMTYPE_CRASH,               // 撞车报警
    CCMS_ALARMTYPE_HELP,                // 求助报警
    CCMS_ALARMTYPE_REQUESTINFORMATION,  // 请求信息咨询报警
    CCMS_ALARMTYPE_KLAXON,              // 汽车喇叭报警
    CCMS_ALARMTYPE_IDLE,                // 怠速报警
    CCMS_ALARMTYPE_TRAILER,             // 拖车报警
    CCMS_ALARMTYPE_FORBIDDENTIME,       // 禁止行驶报警
    CCMS_ALARMTYPE_HIGHTEMPERATURE,     // 温度过高报警
    CCMS_ALARMTYPE_LOWTEMPERATURE,      // 温度过低报警
    CCMS_ALARMTYPE_OILSTOLEN,           // 偷油报警
    CCMS_ALARMTYPE_FIREUP,              // 点火报警
    CCMS_ALARMTYPE_VIBRATION,           // 震动报警
    CCMS_ALARMTYPE_ARREARS              // 欠费报警
} emAlarmType;

//车牌颜色 0：黄 1：黑  2：蓝  3：白
typedef enum emColorTypeTag
{
    CCMS_COLORTYPE_YELLOW = 0,
    CCMS_COLORTYPE_BLACK,
    CCMS_COLORTYPE_BLUE,
    CCMS_COLORTYPE_WHITE
} emColorType;


#ifdef __cplusplus
}
#endif

#endif /* _CCMS_H_ */
