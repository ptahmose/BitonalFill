#pragma once

#include "config.h"
#include <cstdint>

/// The source bitmap (assumed to have pixeltype Gray16) is copied to the destination memory block,
/// where each word (in the source) is "hi-lo-byte-unpacked", meaning that the lo-bytes of every word
/// are stored consecutively in the destination, followed by the hi-bytes of overy word.
///
/// \param          width   The width of the source bitmap.
/// \param          height  The height of the source bitmap.
/// \param          stride  The stride of the source bitmap in bytes.
/// \param          source  Pointer to the source bitmap.
/// \param [in,out] dest    Pointer to the destination buffer. The size of this buffer must be at least (width * height * 2) bytes.
void LoHiByteUnpack_C(
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    const void* source,
    void* dest);

/// The data in the source buffer (which is hi-lo-byte unpacked) is converted into a bitmap of pixeltype Gray16.
///
/// \param          ptrSrc  The pointer to the "hi-lo-byte unpacked" source buffer.
/// \param          sizeSrc The size of the source data.
/// \param          width   The width of the destination bitmap.
/// \param          height  The height of the destination bitmap.
/// \param          stride  The stride of the destination bitmap in bytes.
/// \param [in,out] dest    Pointer to the destination bitmap.
void LoHiBytePack_C(
    const void* ptrSrc,
    size_t sizeSrc,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    void* dest);

#if BITONALFILL_HASAVX
void LoHiByteUnpack_AVX(
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    const void* source,
    void* dest);

void LoHiBytePack_AVX(
    const void* ptrSrc,
    size_t sizeSrc,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    void* dest);
#endif

#if BITONALFILL_HASNEON
void LoHiByteUnpack_NEON(
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    const void* source,
    void* dest);

void LoHiBytePack_NEON(
    const void* ptrSrc,
    size_t sizeSrc,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t stride,
    void* dest);
#endif