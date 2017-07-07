/*
 * CommonPackage.h
 *
 *  Created on: 2016年6月29日
 *      Author: wong
 */

#ifndef PACKAGE_COMMONPACKAGE_H_
#define PACKAGE_COMMONPACKAGE_H_

#include "Header.h"

class LoginPackage : public Package
{
public:
    LoginPackage();
    virtual ~LoginPackage();

protected:
    virtual size_t GetBodyLen();
    virtual int EncodeBody(char** buf, const size_t len);
    virtual int DecodeBody(const char** buf, const size_t len);
    virtual std::string BodyToString();

public:
    uint8_t username[32];
    uint8_t password[32];
};

class LoginRespPackage : public Package
{
public:
    enum {
        LOGIN_Success       = 0,
        LOGIN_UserNotExist  = 1,
        LOGIN_PwdMismatch   = 2,
        LOGIN_OtherError    = 3
    };
public:
    LoginRespPackage();
    virtual ~LoginRespPackage(){}
};

class LogoutPackage : public Package
{
public:
    LogoutPackage();
    virtual ~LogoutPackage(){}

protected:
    virtual size_t GetBodyLen();
    virtual int EncodeBody(char** buf, const size_t len);
    virtual int DecodeBody(const char** buf, const size_t len);
    //virtual std::string BodyToString();

public:
    uint8_t username[32];
};

class LogoutRespPackage : public Package
{
public:
    enum {
        LOGOUT_Success      = 0,
        LOGOUT_UserNotExist = 1,
        LOGOUT_OtherError   = 2
    };
public:
    LogoutRespPackage();
    virtual ~LogoutRespPackage(){}
};

class KeepAlivePackage : public Package
{
public:
    KeepAlivePackage();
    virtual ~KeepAlivePackage(){}
};

//class FEPCommonRespPackage : public Package
//{
//public:
//    enum {
//        RESULT_Ok           = 0, // 成功/确认
//        RESULT_Fail         = 1, // 失败
//        RESULT_MsgInvalid   = 2, // 消息有误
//        RESULT_NotSupport   = 3, // 不支持
//    };
//public:
//    FEPCommonRespPackage();
//    virtual ~FEPCommonRespPackage();
//
//public:
//    uint16_t u2AckMsgId;
//    uint8_t u1Result;
//};

//class PlatformCommonRespPackage : public Package
//{
//public:
//    enum {
//        RESULT_Ok           = 0, // 成功/确认
//        RESULT_Fail         = 1, // 失败
//        RESULT_MsgInvalid   = 2, // 消息有误
//        RESULT_NotSupport   = 3, // 不支持
//        RESULT_AlertConfirm = 4  // 报警处理确认
//    };
//public:
//    PlatformCommonRespPackage();
//    virtual ~PlatformCommonRespPackage();
//
//public:
//    uint16_t u2AckMsgId;
//    uint8_t u1Result;
//};

class CommonPackage : public Package
{
public:
    CommonPackage();
    virtual ~CommonPackage();

protected:
    virtual size_t GetBodyLen();
    virtual int EncodeBody(char** buf, const size_t len);
    virtual int DecodeBody(const char** buf, const size_t len);

public:
    uint8_t* u1DataPtr;

private:
    uint8_t* _p;

private:
    CommonPackage(const CommonPackage&);
    CommonPackage &operator=(const CommonPackage&);
};

#endif /* PACKAGE_COMMONPACKAGE_H_ */
