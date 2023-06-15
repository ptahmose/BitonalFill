#pragma once

#include "CopyWithBitonalMask.h"

#include <cstdint>
#include <cstddef>

int CheckCopyWithBitonalMask_Roi_Parameters(
    std::uint32_t width, 
    std::uint32_t height, 
    std::uint32_t roi_x,
    std::uint32_t roi_y,
    std::uint32_t roi_width,
    std::uint32_t roi_height);

int CheckCopyWithBitonalMask_BitonalSource_Parameters(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride);

int CheckCopyWithBitonalMask_Bitmap_Parameters(
    std::uint32_t width,
    std::uint32_t height,
    const void* bitmap_data,
    std::uint32_t bitmap_stride,
    size_t size_of_pixel);