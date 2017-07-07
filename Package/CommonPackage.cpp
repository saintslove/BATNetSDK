/*
 * CommonPackage.cpp
 *
 *  Created on: 2016年6月29日
 *      Author: wong
 */

#include "CommonPackage.h"

#include <cassert>
#include <sstream>
#include <cstring>

#include "Common.h"
#include "Encoder.h"

LoginPackage::LoginPackage()
: Package(MSG_Login, "LoginPackage")
{
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
}

LoginPackage::~LoginPackage()
{
}

size_t LoginPackage::GetBodyLen()
{
    return sizeof(username) + sizeof(password);
}

int LoginPackage::EncodeBody(char** buf, const size_t len)
{
    char* p = *buf;
    ENCODE_ARRAY(username, &p);
    ENCODE_ARRAY(password, &p);
    assert(p - (*buf) == static_cast<int>(GetBodyLen()));
    (*buf) += GetBodyLen();
    return EC_Ok;
}

int LoginPackage::DecodeBody(const char** buf, const size_t len)
{
    const char* p = *buf;
    DECODE_ARRAY(&p, username);
    DECODE_ARRAY(&p, password);
    assert(p - (*buf) == static_cast<int>(GetBodyLen()));
    (*buf) += GetBodyLen();
    return EC_Ok;
}

std::string LoginPackage::BodyToString()
{
    std::stringstream stream;
    stream << ">username: " << ARRAY2STR(username)
        << "\n>password: " << ARRAY2STR(password);
    return stream.str();
}

LoginRespPackage::LoginRespPackage()
: Package(MSG_LoginResp, "LoginRespPackage")
{
}

LogoutPackage::LogoutPackage()
: Package(MSG_Logout, "LogoutPackage")
{
    memset(username, 0, sizeof(username));
}

size_t LogoutPackage::GetBodyLen()
{
    return sizeof(username);
}

int LogoutPackage::EncodeBody(char** buf, const size_t len)
{
    char* p = *buf;
    ENCODE_ARRAY(username, &p);
    assert(p - (*buf) == static_cast<int>(GetBodyLen()));
    (*buf) += GetBodyLen();
    return EC_Ok;
}

int LogoutPackage::DecodeBody(const char** buf, const size_t len)
{
    const char* p = *buf;
    DECODE_ARRAY(&p, username);
    assert(p - (*buf) == static_cast<int>(GetBodyLen()));
    (*buf) += GetBodyLen();
    return EC_Ok;
}

LogoutRespPackage::LogoutRespPackage()
: Package(MSG_LogoutResp, "LogoutRespPackage")
{
}

KeepAlivePackage::KeepAlivePackage()
: Package(MSG_KeepAlive, "KeepAlivePackage")
{
}

CommonPackage::CommonPackage()
: Package(MSG_Unknown, "CommonPackage")
, u1DataPtr(NULL)
, _p(NULL)
{
}

CommonPackage::~CommonPackage()
{
    assert(_p == NULL || _p == u1DataPtr);
    SAFE_DELETEA(_p);
    u1DataPtr = NULL;
}

size_t CommonPackage::GetBodyLen()
{
    return header.u4BodyLen;
}

int CommonPackage::EncodeBody(char** buf, const size_t len)
{
    char* p = *buf;
    Encoder::Encode(u1DataPtr, header.u4BodyLen, &p);
    assert(p - (*buf) == static_cast<int>(GetBodyLen()));
    (*buf) += GetBodyLen();
    return EC_Ok;
}

int CommonPackage::DecodeBody(const char** buf, const size_t len)
{
    const char* p = *buf;
    assert(header.u4BodyLen > 0);
    assert(header.u4BodyLen < 65536);
    u1DataPtr = _p = new uint8_t[header.u4BodyLen];
    assert(_p != NULL);
    assert(u1DataPtr == _p);
    Encoder::Decode(&p, header.u4BodyLen, u1DataPtr);
    assert(p - (*buf) == static_cast<int>(GetBodyLen()));
    assert(p - (*buf) <= static_cast<int>(len));
    (*buf) += GetBodyLen();
    return EC_Ok;
}
