/*
 * Header.cpp
 *
 *  Created on: 2016年6月29日
 *      Author: wong
 */

#include "Header.h"

#include <cassert>
#include <sstream>
#include <cstring>

#include "Encoder.h"
/*
//////////////////////////////////////////////////////////////////////
// Header
Header::Header()
: u4Head(PACKAGE_MAGIC_NUMBER)
, u4Version(PACKAGE_CUR_VERSION)
, u2DeviceType(DT_Unknown)
, u4SeqNo(0)
, u4AckNo(0)
, u4MessageId(0)
, u4SessionId(0)
, u4BodyLen(0)
, u8Timestamp(0)
, u4Reserved(0)
{
}

Header::~Header()
{
}

bool Header::CheckParam(const char* buf, const size_t len)
{
    return (buf != NULL) && (len >= GetHeaderLen());
}

bool Header::Valid()
{
    return u4Head == PACKAGE_MAGIC_NUMBER
        && u4Version == PACKAGE_CUR_VERSION
        && u4BodyLen < PACKAGE_MAX_LENGTH;
}

size_t Header::GetHeaderLen()
{
    return sizeof(u4Head)
        + sizeof(u4Version)
        + sizeof(u2DeviceType)
        + sizeof(u4SeqNo)
        + sizeof(u4AckNo)
        + sizeof(u4MessageId)
        + sizeof(u4SessionId)
        + sizeof(u4BodyLen)
        + sizeof(u8Timestamp)
        + sizeof(u4Reserved);
}

int Header::Encode(char* buf, const size_t len)
{
    if (!CheckParam(buf, len))
    {
        return EC_InvalidParam;
    }

    void* p = static_cast<void*>(buf);
    Encoder::Encode(u4Head, p);
    Encoder::Encode(u4Version, p);
    Encoder::Encode(u2DeviceType, p);
    Encoder::Encode(u4SeqNo, p);
    Encoder::Encode(u4AckNo, p);
    Encoder::Encode(u4MessageId, p);
    Encoder::Encode(u4SessionId, p);
    Encoder::Encode(u4BodyLen, p);
    Encoder::Encode(u8Timestamp, p);
    Encoder::Encode(u4Reserved, p);
    assert(p - buf == GetHeaderLen());
    buf += GetHeaderLen();
    return EC_Ok;
}

int Header::Decode(const char* buf, const size_t len)
{
    if (!CheckParam(buf, len))
    {
        return EC_InvalidParam;
    }

    void* p = static_cast<void*>(buf);
    Encoder::Decode(p, u4Head);
    Encoder::Decode(p, u4Version);
    Encoder::Decode(p, u2DeviceType);
    Encoder::Decode(p, u4SeqNo);
    Encoder::Decode(p, u4AckNo);
    Encoder::Decode(p, u4MessageId);
    Encoder::Decode(p, u4SessionId);
    Encoder::Decode(p, u4BodyLen);
    Encoder::Decode(p, u8Timestamp);
    Encoder::Decode(p, u4Reserved);
    assert(p - buf == GetHeaderLen());
    buf += GetHeaderLen();
    return EC_Ok;
}

std::string Header::ToString()
{
    std::stringstream stream;
    stream << "u4Head: " << u4Head
        << "\nu4Version: " << u4Version
        << "\nu2DeviceType: " << u2DeviceType
        << "\nu4SeqNo: " << u4SeqNo
        << "\nu4AckNo: " << u4AckNo
        << "\nu4MessageId: " << u4MessageId
        << "\nu4SessionId: " << u4SessionId
        << "\nu4BodyLen: " << u4BodyLen
        << "\nu8Timestamp: " << u8Timestamp
        << "\nu4Reserved: " << u4Reserved;
    return stream.str();
}*/

//////////////////////////////////////////////////////////////////////
// Package
Package::Package()
: name("Package")
{
    Init();
}

Package::Package(MessageId msgId, const std::string& name)
: name(name)
{
    Init();
    header.u4MessageId = msgId;
}

void Package::Init()
{
    memset(&header, 0, sizeof(header));
    header.u4Head = PACKAGE_MAGIC_NUMBER;
    header.u4Version = PACKAGE_CUR_VERSION;
    header.u2DeviceType = DT_Unknown;
    header.u8Timestamp = time(NULL); // wbf.mark
}

Package::~Package()
{
}

size_t Package::GetHeaderLen()
{
    return sizeof(header);
}

int Package::PeekPackage(const char* buf, const size_t len, uint32_t& msgId)
{
    if (len < GetHeaderLen())
    {
        return EC_Continue;
    }
    const Header* pHead = reinterpret_cast<const Header*>(buf);
    if (pHead->u4Head != PACKAGE_MAGIC_NUMBER
        || pHead->u4Version != PACKAGE_CUR_VERSION
        || pHead->u4BodyLen > PACKAGE_MAX_LENGTH)
    {
        return EC_Fail;
    }
    else if (pHead->u4BodyLen + GetHeaderLen() > len)
    {
        return EC_Continue;
    }
    else
    {
        msgId = pHead->u4MessageId;
        return EC_Ok;
    }
}

bool Package::ValidHeader()
{
    return header.u4Head == PACKAGE_MAGIC_NUMBER
        && header.u4Version == PACKAGE_CUR_VERSION
        && header.u4BodyLen <= PACKAGE_MAX_LENGTH;
}

int Package::EncodeHeader(char** buf, const size_t len)
{
    if (buf == NULL || *buf == NULL)
    {
        return EC_InvalidParam;
    }
    if (len < GetHeaderLen())
    {
        return EC_BufferNotEnough;
    }

    char* p = *buf;
    Encoder::Encode(header.u4Head, &p);
    Encoder::Encode(header.u4Version, &p);
    Encoder::Encode(header.u2DeviceType, &p);
    Encoder::Encode(header.u4SeqNo, &p);
    Encoder::Encode(header.u4AckNo, &p);
    Encoder::Encode(header.u4MessageId, &p);
    Encoder::Encode(header.u4SessionId, &p);
    Encoder::Encode(header.u4BodyLen, &p);
    Encoder::Encode(header.u8Timestamp, &p);
    Encoder::Encode(header.u4Reserved, &p);
    assert(p - (*buf) == static_cast<int>(GetHeaderLen()));
    (*buf) += GetHeaderLen();
    return EC_Ok;
}

int Package::DecodeHeader(const char** buf, const size_t len)
{
    if (buf == NULL || *buf == NULL)
    {
        return EC_InvalidParam;
    }
    if (len < GetHeaderLen())
    {
        return EC_BufferNotEnough;
    }

    uint32_t msgId = header.u4MessageId;

    const char* p = *buf;
    Encoder::Decode(&p, header.u4Head);
    Encoder::Decode(&p, header.u4Version);
    Encoder::Decode(&p, header.u2DeviceType);
    Encoder::Decode(&p, header.u4SeqNo);
    Encoder::Decode(&p, header.u4AckNo);
    Encoder::Decode(&p, header.u4MessageId);
    Encoder::Decode(&p, header.u4SessionId);
    Encoder::Decode(&p, header.u4BodyLen);
    Encoder::Decode(&p, header.u8Timestamp);
    Encoder::Decode(&p, header.u4Reserved);
    assert(p - (*buf) == static_cast<int>(GetHeaderLen()));
    (*buf) += GetHeaderLen();

    assert(msgId == 0 || msgId == header.u4MessageId);

    return EC_Ok;
}

bool Package::Valid()
{
    return ValidHeader();
}

size_t Package::GetLength()
{
    return GetHeaderLen() + GetBodyLen();
}

int Package::Encode(char* buf, const size_t len)
{
    // 打包前先设置包体长度
    header.u4BodyLen = GetBodyLen();

    char** p = &buf;
    int ret = EncodeHeader(p, len);
    if (ret != EC_Ok)
    {
        return ret;
    }

    if (buf == NULL)
    {
        return EC_InvalidParam;
    }
    if (len - GetHeaderLen() < GetBodyLen())
    {
        return EC_BufferNotEnough;
    }
    return EncodeBody(p, len - GetHeaderLen());
}

int Package::Decode(const char* buf, const size_t len)
{
    const char** p = &buf;
    int ret = DecodeHeader(p, len);
    if (ret != EC_Ok)
    {
        return ret;
    }

    if (buf == NULL)
    {
        return EC_InvalidParam;
    }
    if (len - GetHeaderLen() < GetBodyLen())
    {
        return EC_BufferNotEnough;
    }
    return DecodeBody(p, len - GetHeaderLen());
}

std::string Package::GetName()
{
    return name;
}

std::string Package::ToString()
{
    return HeaderToString()
        + "\n=====================Body=====================\n"
        + BodyToString();
}

std::string Package::HeaderToString()
{
    std::stringstream stream;
    stream << "====================Header===================="
        << "\n>u4Head: " << header.u4Head
        << "\n>u4Version: " << header.u4Version
        << "\n>u2DeviceType: " << header.u2DeviceType
        << "\n>u4SeqNo: " << header.u4SeqNo
        << "\n>u4AckNo: " << header.u4AckNo
        << "\n>u4MessageId: " << header.u4MessageId
        << "\n>u4SessionId: " << header.u4SessionId
        << "\n>u4BodyLen: " << header.u4BodyLen
        << "\n>u8Timestamp: " << header.u8Timestamp
        << "\n>u4Reserved: " << header.u4Reserved;
    return stream.str();
}
