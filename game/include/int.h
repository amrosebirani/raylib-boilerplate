#pragma once

#include <stdint.h>

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

static inline uint16 swapuint16(uint16 x) {
    return (x >> 8) | (x << 8);
}

static inline uint32 swapuint32(uint32 x) {
    return ((x & 0x000000FF) << 24) | ((x & 0x0000FF00) << 8) |
           ((x & 0x00FF0000) >> 8) | ((x & 0xFF000000) >> 24);
}

static inline uint64 swapuint64(uint64 x) {
    return ((x << 56) & 0xFF00000000000000ULL) |
           ((x << 40) & 0x00FF000000000000ULL) |
           ((x << 24) & 0x0000FF0000000000ULL) |
           ((x << 8) & 0x000000FF00000000ULL) |
           ((x >> 8) & 0x00000000FF000000ULL) |
           ((x >> 24) & 0x0000000000FF0000ULL) |
           ((x >> 40) & 0x000000000000FF00ULL) |
           ((x >> 56) & 0x00000000000000FFULL);
}
