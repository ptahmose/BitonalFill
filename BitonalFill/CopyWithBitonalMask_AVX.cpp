#include "config.h"

#if BITONALFILL_HASAVX

#ifndef __AVX__ 
    #error "AVX instructions are not enabled - please add the option /arch:AVX when compiling this file"
#endif

#include <immintrin.h>
#include <algorithm>
#include "CopyWithBitonalMask.h"
#include "CopyWithBitonalMaskUtilities.h"

using namespace std;

static const __m128i kBitSelectMask = _mm_set_epi8(0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80), 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80));
static const __m128i kShuffleConstForRepeatBytes8Times = _mm_set_epi8(0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

template<typename T> inline void DoPixelsUntilAligned(uint32_t start_bit_no, uint32_t end_bit_no, const uint8_t*& bitonal_mask, const T*& source, T*& destination)
{
    const uint8_t bitonal_byte = *bitonal_mask;
    for (uint32_t i = start_bit_no; i < end_bit_no; ++i)
    {
        const uint8_t bit = 0x80 >> i;
        if (bitonal_byte & bit)
        {
            *destination = *source;
        }

        ++source;
        ++destination;
    }

    ++bitonal_mask;
}

template<typename T> inline void DoRemainingPixels(uint32_t width_remainder, const uint8_t* bitonal_mask, const T*& source, T*& destination)
{
    for (uint32_t n = 0; n < width_remainder; ++n)
    {
        const uint8_t bitonal = *(bitonal_mask + n / 8);
        const uint8_t bit = 0x80 >> (n % 8);
        if (bitonal & bit)
        {
            *destination = *source;
        }

        ++source;
        ++destination;
    }
}

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
    std::uint32_t destination_stride)
{
    int error_code = CheckCopyWithBitonalMask_Roi_Parameters(width, height, roi_x, roi_y, roi_width, roi_height);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_BitonalSource_Parameters(width, height, source_bitonal, source_bitonal_stride);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_Bitmap_Parameters(width, height, source, source_stride, 1);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_Bitmap_Parameters(roi_width, roi_height, destination, destination_stride, 1);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    const __m128i zero = _mm_setzero_si128();

    const uint8_t* source_pointer = source + static_cast<size_t>(roi_y) * source_stride + static_cast<size_t>(roi_x);
    const uint8_t* bitonal_pointer = source_bitonal + static_cast<size_t>(roi_y) * source_bitonal_stride + roi_x / 8;

    // We are given the a ROI, of interest here is roi_x and roi_width. The code below
    // first calculates the number of bits so that we are byte aligned (in the bitonal mask).
    // Those bits we do in the first part, then we do the remaining "full bytes" with AVX-code
    // (i.e. we do 16 pixels at once, i.e. two bytes from the bitonal mask). In the third part
    // we then do any remaining bits, again with with scalar code.
    // +-----------------+--------|
    // |xxxx1010|10010011|011xxxxx|
    // +-----------------+--------|

    // This is the number of bits we need to do before we are byte aligned - we round up the roi_x to the next byte boundary, and then subtract roi_x.
    const uint32_t bits_to_byte_alignment = ((roi_x + 7) / 8) * 8 - roi_x;
    const uint32_t start_bit_no = roi_x % 8;    // at what bit to start in the first byte (if we need to do bits_to_byte_alignment bits before we are byte aligned)
    const uint32_t end_bit_no = (min)(8u - start_bit_no, roi_width + start_bit_no);   // note: start_bit_no cannot be larger than 8, but we must ensure that we never do more bits than roi_width specifies

    // now, we only have to consider the remaining bits, i.e. the number of bits calculated above needs to be subtracted from the roi_width.
    const uint32_t remaining_bits_count = roi_width > bits_to_byte_alignment ? roi_width - bits_to_byte_alignment : 0;
    const uint32_t width_over16 = remaining_bits_count / 16;
    const uint32_t width_remainder = remaining_bits_count % 16;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* bitonal_line = bitonal_pointer + static_cast<size_t>(y) * source_bitonal_stride;
        const uint8_t* source_line = source_pointer + static_cast<size_t>(y) * source_stride;
        uint8_t* destination_line = destination + static_cast<size_t>(y) * destination_stride;

        // First, we do the bits until we are byte aligned.
        if (bits_to_byte_alignment > 0)
        {
            DoPixelsUntilAligned(start_bit_no, end_bit_no, bitonal_line, source_line, destination_line);
        }

        // ...then, we do the remaining bits in 16 pixel chunks
        for (uint32_t x16 = 0; x16 < width_over16; ++x16)
        {
            const __m128i value = _mm_loadu_si128(reinterpret_cast<const __m128i*>(source_line));
            // load two bytes from the bitonal source, and repeat each byte 8 times
            // so, we get bitonal = src[0] src[0] src[0] src[0] src[0] src[0] src[0] src[0] | src[1] src[1] src[1] src[1] src[1] src[1] src[1] src[1]
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(*reinterpret_cast<const short*>(bitonal_line)), kShuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _mm_andnot_si128(bitonal, kBitSelectMask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);
            _mm_maskmoveu_si128(value, maskForStore, reinterpret_cast<char*>(destination_line));

            bitonal_line += 2;
            source_line += 16;
            destination_line += 16;
        }

        // ...and finally, if there is a remainder, we need to do it the slow way. The remainder is always less than 16.
        DoRemainingPixels(width_remainder, bitonal_line, source_line, destination_line);
    }

    return ReturnCode_Success;
}

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
    std::uint32_t destination_stride)
{
    int error_code = CheckCopyWithBitonalMask_Roi_Parameters(width, height, roi_x, roi_y, roi_width, roi_height);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_BitonalSource_Parameters(width, height, source_bitonal, source_bitonal_stride);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_Bitmap_Parameters(width, height, source, source_stride, 2);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_Bitmap_Parameters(roi_width, roi_height, destination, destination_stride, 2);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    const __m128i zero = _mm_setzero_si128();

    const uint8_t* source_pointer = reinterpret_cast<const uint8_t*>(source) + static_cast<size_t>(roi_y) * source_stride + static_cast<size_t>(roi_x) * sizeof(uint16_t);
    const uint8_t* bitonal_pointer = source_bitonal + static_cast<size_t>(roi_y) * source_bitonal_stride + roi_x / 8;

    // This is the number of bits we need to do before we are byte aligned - we round up the roi_x to the next byte boundary, and then subtract roi_x.
    const uint32_t bits_to_byte_alignment = ((roi_x + 7) / 8) * 8 - roi_x;
    const uint32_t start_bit_no = roi_x % 8;    // at what bit to start in the first byte (if we need to do bits_to_byte_alignment bits before we are byte aligned)
    const uint32_t end_bit_no = (min)(8u - start_bit_no, roi_width + start_bit_no);   // note: start_bit_no cannot be larger than 8, but we must ensure that we never do more bits than roi_width specifies

    // now, we only have to consider the remaining bits, i.e. the number of bits calculated above needs to be subtracted from the roi_width.
    const uint32_t remaining_bits_count = roi_width > bits_to_byte_alignment ? roi_width - bits_to_byte_alignment : 0;
    const uint32_t width_over16 = remaining_bits_count / 16;
    const uint32_t width_remainder = remaining_bits_count % 16;

    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* bitonal_line = bitonal_pointer + static_cast<size_t>(y) * source_bitonal_stride;
        const uint16_t* source_line = reinterpret_cast<const uint16_t*>(source_pointer + static_cast<size_t>(y) * source_stride);
        uint16_t* destination_line = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destination_stride);

        // First, we do the bits until we are byte aligned.
        if (bits_to_byte_alignment > 0)
        {
            DoPixelsUntilAligned(start_bit_no, end_bit_no, bitonal_line, source_line, destination_line);
        }

        // ...then, we do the remaining bits in 16 pixel chunks
        for (uint32_t x16 = 0; x16 < width_over16; ++x16)
        {
            const __m128i value1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(source_line));
            const __m128i value2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(source_line + 8));

            // load two bytes from the bitonal source, and repeat each byte 8 times
            // so, we get bitonal = src[0] src[0] src[0] src[0] src[0] src[0] src[0] src[0] | src[1] src[1] src[1] src[1] src[1] src[1] src[1] src[1]
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(*reinterpret_cast<const short*>(bitonal_line)), kShuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i and_with_mask = _mm_andnot_si128(bitonal, kBitSelectMask);
            const __m128i mask_for_store = _mm_cmpeq_epi8(and_with_mask, zero);

            // extend bytes to words, so 0x00 -> 0x0000 and 0xff -> 0xffff
            const __m128i mask_for_store_words_low = _mm_cvtepi8_epi16(mask_for_store);
            const __m128i mask_for_store_words_high = _mm_cvtepi8_epi16(_mm_unpackhi_epi64(mask_for_store, mask_for_store));

            _mm_maskmoveu_si128(value1, mask_for_store_words_low, reinterpret_cast<char*>(destination_line));
            _mm_maskmoveu_si128(value2, mask_for_store_words_high, reinterpret_cast<char*>(destination_line + 8));

            bitonal_line += 2;
            source_line += 16;
            destination_line += 16;
        }

        // If there is a remainder, we need to do it the slow way. The remainder is always less than 16.
        DoRemainingPixels(width_remainder, bitonal_line, source_line, destination_line);
    }

    return ReturnCode_Success;
}

#endif