/*
 * Encoder.h
 *
 *  Created on: 2016年6月30日
 *      Author: wong
 */

#ifndef PACKAGE_ENCODER_H_
#define PACKAGE_ENCODER_H_

#include <inttypes.h>
#include <cstddef>

#define ENCODE_ARRAY(array, buf) \
    Encoder::Encode(array, sizeof(array), buf)

#define DECODE_ARRAY(buf, array) \
    Encoder::Decode(buf, sizeof(array), array)

class Encoder
{
public:
    static void Encode(const uint8_t value, char** buf);
    static void Encode(const uint16_t value, char** buf);
    static void Encode(const uint32_t value, char** buf);
    static void Encode(const uint64_t value, char** buf);
    static void Encode(const uint8_t* value, const size_t size, char** buf);
    static void Encode(const uint16_t* value, const size_t size, char** buf);
    static void Encode(const uint32_t* value, const size_t size, char** buf);
    static void Encode(const uint64_t* value, const size_t size, char** buf);
    static void Encode(const double value, char** buf);

private:
    static inline void EncodeImpl(const void* value, const size_t size, char** buf);

public:
    static void Decode(const char** buf, uint8_t& value);
    static void Decode(const char** buf, uint16_t& value);
    static void Decode(const char** buf, uint32_t& value);
    static void Decode(const char** buf, uint64_t& value);
    static void Decode(const char** buf, const size_t size, uint8_t* value);
    static void Decode(const char** buf, const size_t size, uint16_t* value);
    static void Decode(const char** buf, const size_t size, uint32_t* value);
    static void Decode(const char** buf, const size_t size, uint64_t* value);
    static void Decode(const char** buf, double& value);

private:
    static inline void DecodeImpl(const char** buf, const size_t size, void* value);
};

#endif /* PACKAGE_ENCODER_H_ */
