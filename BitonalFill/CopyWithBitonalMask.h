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

/// Copies a rectangular region from a source bitmap to a destination bitmap, using a bitonal mask to determine which pixels to copy.
///
/// \param          width                   The width of the source bitmap (and the source mask) in pixels.
/// \param          height                  The height of the source bitmap (and the source mask) in pixels.
/// \param          source_bitonal          Pointer to the bitonal mask.
/// \param          source_bitonal_stride   The stride of the bitonal mask (in bytes).
/// \param          source                  Pointer to the source bitmap (which has width 'width' and height 'height').
/// \param          source_stride           The stride of the source bitmap (in byteS).
/// \param          roi_x                   The x-coordinate of the top-left corner of the region-of-interest.
/// \param          roi_y                   The y-coordinate of the top-left corner of the region-of-interest.
/// \param          roi_width               The width of the region-of-interest.
/// \param          roi_height              The height of the region-of-interest.
/// \param [out]    destination             Pointer to the destination bitmap (which has width 'roi_width' and height 'roi_height').
/// \param          destination_stride      The stride of the destination bitmap.
void CopyWithBitonalMask_Roi_Gray8_C(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint8_t* source,
    std::uint32_t source_stride,
    std::uint32_t roi_x,
    std::uint32_t roi_y,
    std::uint32_t roi_width,
    std::uint32_t roi_height,
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