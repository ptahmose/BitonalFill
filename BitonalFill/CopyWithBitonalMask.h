#pragma once

#include <cstdint>

#include "config.h"

void CopyWithBitonalMask_Gray8_C(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint8_t* source,
    std::uint32_t source_stride,
    std::uint8_t* destination, 
    std::uint32_t destination_stride);

void CopyWithBitonalMask_Gray16_C(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint16_t* destination, 
    std::uint32_t destination_stride);


#if BITONALFILL_HASAVX
void CopyWithBitonalMask_Gray8_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint8_t* source,
    std::uint32_t source_stride,
    std::uint8_t* destination, 
    std::uint32_t destination_stride);

void CopyWithBitonalMask_Gray16_AVX(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint16_t* destination, 
    std::uint32_t destination_stride);
#endif