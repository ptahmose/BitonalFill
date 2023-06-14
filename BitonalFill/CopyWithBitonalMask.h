#pragma once

// ReSharper disable CppClangTidyClangDiagnosticInvalidUtf8

#include <cstdint>

#include "config.h"

const int ReturnCode_Success = 0;
const int ReturnCode_InvalidArgument = 1;
const int ReturnCode_InvalidRoi = 2;
const int ReturnCode_InvalidPointers = 3;
const int ReturnCode_InvalidStride = 4;

/// Copies a rectangular region from a source bitmap to a destination bitmap, using a bitonal mask to determine which pixels to copy.
/// The specified ROI must be fully contained with the source.
/// We have this situation for the source:
/// 
///            width
///     ┌──────────────────┐
///     │                  │
///     │ (roi_x,roi_y)    │
///     │   ╔══════════╗r  │h
///     │   ║          ║o  │e
///     │   ║          ║i  │i
///     │   ║          ║_  │g
///     │   ║ roi_width║h  │h
///     │   ╚══════════╝e  │t
///     │               i  │
///     │               g  │
///     │               h  │
///     │               t  │
///     └──────────────────┘
/// 
/// The ROI from the source is copied to a destination bitmap of width = roi_width and height = roi_height.
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
/// \returns                                An integer indicating failure or success, c.f. the constants ReturnCode_XXX.
int CopyWithBitonalMask_Roi_Gray8_C(
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

/// Copies a rectangular region from a source bitmap to a destination bitmap, using a bitonal mask to determine which pixels to copy.
/// The specified ROI must be fully contained with the source.
/// We have this situation for the source:
/// 
///            width
///     ┌──────────────────┐
///     │                  │
///     │ (roi_x,roi_y)    │
///     │   ╔══════════╗r  │h
///     │   ║          ║o  │e
///     │   ║          ║i  │i
///     │   ║          ║_  │g
///     │   ║ roi_width║h  │h
///     │   ╚══════════╝e  │t
///     │               i  │
///     │               g  │
///     │               h  │
///     │               t  │
///     └──────────────────┘
/// 
/// The ROI from the source is copied to a destination bitmap of width = roi_width and height = roi_height.
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
/// \returns                                An integer indicating failure or success, c.f. the constants ReturnCode_XXX.
int CopyWithBitonalMask_Roi_Gray16_C(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint32_t roi_x,
    std::uint32_t roi_y,
    std::uint32_t roi_width,
    std::uint32_t roi_height,
    std::uint16_t* destination, 
    std::uint32_t destination_stride);

inline int CopyWithBitonalMask_Gray8_C(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint8_t* source,
    std::uint32_t source_stride,
    std::uint8_t* destination, 
    std::uint32_t destination_stride)
{
    return CopyWithBitonalMask_Roi_Gray8_C(
        width, height,
        source_bitonal, source_bitonal_stride,
        source, source_stride,
        0, 0, width, height,
        destination, destination_stride);
}

inline int CopyWithBitonalMask_Gray16_C(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint16_t* destination, 
    std::uint32_t destination_stride)
{
    return CopyWithBitonalMask_Roi_Gray16_C(
        width, height,
        source_bitonal, source_bitonal_stride,
        source, source_stride,
        0, 0, width, height,
        destination, destination_stride);
}

#if BITONALFILL_HASAVX
int CopyWithBitonalMask_Roi_Gray8_AVX(
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

int CopyWithBitonalMask_Roi_Gray16_AVX(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint32_t roi_x,
    std::uint32_t roi_y,
    std::uint32_t roi_width,
    std::uint32_t roi_height,
    std::uint16_t* destination, 
    std::uint32_t destination_stride);

inline int CopyWithBitonalMask_Gray8_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint8_t* source,
    std::uint32_t source_stride,
    std::uint8_t* destination, 
    std::uint32_t destination_stride)
{
    return CopyWithBitonalMask_Roi_Gray8_AVX(
        width, height,
        source_bitonal, source_bitonal_stride,
        source, source_stride,
        0, 0, width, height,
        destination, destination_stride);
}

inline int CopyWithBitonalMask_Gray16_AVX(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint16_t* destination, 
    std::uint32_t destination_stride)
{
    return CopyWithBitonalMask_Roi_Gray16_AVX(
        width, height,
        source_bitonal, source_bitonal_stride,
        source, source_stride,
        0, 0, width, height,
        destination, destination_stride);
}

#endif