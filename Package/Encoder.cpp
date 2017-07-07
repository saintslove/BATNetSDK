/*
 * Encoder.cpp
 *
 *  Created on: 2016年6月30日
 *      Author: wong
 */

#include "Encoder.h"
#include <cassert>
#include <cstring>

void Encoder::Encode(const uint8_t value, char** buf)
{
    EncodeImpl(static_cast<const void*>(&value), sizeof(value), buf);
}

void Encoder::Encode(const uint16_t value, char** buf)
{
    EncodeImpl(static_cast<const void*>(&value), sizeof(value), buf);
}

void Encoder::Encode(const uint32_t value, char** buf)
{
    EncodeImpl(static_cast<const void*>(&value), sizeof(value), buf);
}

void Encoder::Encode(const uint64_t value, char** buf)
{
    EncodeImpl(static_cast<const void*>(&value), sizeof(value), buf);
}

void Encoder::Encode(const uint8_t* value, const size_t size, char** buf)
{
    EncodeImpl(static_cast<const void*>(value), size, buf);
}

void Encoder::Encode(const uint16_t* value, const size_t size, char** buf)
{
    EncodeImpl(static_cast<const void*>(value), size, buf);
}

void Encoder::Encode(const uint32_t* value, const size_t size, char** buf)
{
    EncodeImpl(static_cast<const void*>(value), size, buf);
}

void Encoder::Encode(const uint64_t* value, const size_t size, char** buf)
{
    EncodeImpl(static_cast<const void*>(value), size, buf);
}

void Encoder::Encode(const double value, char** buf)
{
    assert(sizeof(value) == 8);
    EncodeImpl(static_cast<const void*>(&value), sizeof(value), buf);
}

void Encoder::EncodeImpl(const void* value, const size_t size, char** buf)
{
    assert(value != NULL);
    assert(buf != NULL);
    assert(*buf != NULL);
    assert(size < 65536);
    memcpy(*buf, value, size);
    (*buf) += size;
}


//////////////////////////////////////////////////////////////////////
void Encoder::Decode(const char** buf, uint8_t& value)
{
    DecodeImpl(buf, sizeof(value), static_cast<void*>(&value));
}

void Encoder::Decode(const char** buf, uint16_t& value)
{
    DecodeImpl(buf, sizeof(value), static_cast<void*>(&value));
}

void Encoder::Decode(const char** buf, uint32_t& value)
{
    DecodeImpl(buf, sizeof(value), static_cast<void*>(&value));
}

void Encoder::Decode(const char** buf, uint64_t& value)
{
    DecodeImpl(buf, sizeof(value), static_cast<void*>(&value));
}

void Encoder::Decode(const char** buf, const size_t size, uint8_t* value)
{
    DecodeImpl(buf, size, static_cast<void*>(value));
}

void Encoder::Decode(const char** buf, const size_t size, uint16_t* value)
{
    DecodeImpl(buf, size, static_cast<void*>(value));
}

void Encoder::Decode(const char** buf, const size_t size, uint32_t* value)
{
    DecodeImpl(buf, size, static_cast<void*>(value));
}

void Encoder::Decode(const char** buf, const size_t size, uint64_t* value)
{
    DecodeImpl(buf, size, static_cast<void*>(value));
}

void Encoder::Decode(const char** buf, double& value)
{
    assert(sizeof(value) == 8);
    DecodeImpl(buf, sizeof(value), static_cast<void*>(&value));
}

void Encoder::DecodeImpl(const char** buf, const size_t size, void* value)
{
    assert(buf != NULL);
    assert(*buf != NULL);
    assert(value != NULL);
    assert(size < 65536);
    memcpy(value, *buf, size);
    (*buf) += size;
}
