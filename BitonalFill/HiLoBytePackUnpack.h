#pragma once

#include "config.h"
#include <cstdint>

void LoHiByteUnpack_C(
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    const void* source,
    void* dest);

#if BITONALFILL_HASAVX
void LoHiByteUnpack_AVX(
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    const void* source,
    void* dest);
#endif

#if BITONALFILL_HASNEON
void LoHiByteUnpack_NEON(
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    const void* source,
    void* dest);
#endif