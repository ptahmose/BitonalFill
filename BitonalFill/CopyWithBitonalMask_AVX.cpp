#include "config.h"

#if BITONALFILL_HASAVX 

#include <immintrin.h>
#include <algorithm>
#include "CopyWithBitonalMask.h"
#include "ColorPixelStructs.h"

static const __m128i kBitSelectMask = _mm_set_epi8(0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80), 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80));
static const __m128i kShuffleConstForRepeatBytes8Times = _mm_set_epi8(0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

void CopyWithBitonalMask_Gray8_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint8_t* source,
    std::uint32_t source_stride,
    std::uint8_t* destination,
    std::uint32_t destination_stride)
{
    const __m128i zero = _mm_setzero_si128();

    const uint32_t width_over16 = width / 16;
    const uint32_t width_remainder = width % 16;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* bitonal_line = reinterpret_cast<const uint16_t*>(source_bitonal + static_cast<size_t>(y) * source_bitonal_stride);
        const uint8_t* source_line = source + static_cast<size_t>(y) * source_stride;
        uint8_t* destination_line = destination + static_cast<size_t>(y) * destination_stride;

        for (uint32_t x16 = 0; x16 < width_over16; ++x16)
        {
            const __m128i value = _mm_loadu_si128(reinterpret_cast<const __m128i*>(source_line));
            // load two bytes from the bitonal source, and repeat each byte 8 times
            // so, we get bitonal = src[0] src[0] src[0] src[0] src[0] src[0] src[0] src[0] | src[1] src[1] src[1] src[1] src[1] src[1] src[1] src[1]
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*bitonal_line)), kShuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _mm_andnot_si128(bitonal, kBitSelectMask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);
            _mm_maskmoveu_si128(value, maskForStore, reinterpret_cast<char*>(destination_line));

            ++bitonal_line;
            source_line += 16;
            destination_line += 16;
        }

        // If there is a remainder, we need to do it the slow way. The remainder is always less than 16.
        for (uint32_t n = 0; n < width_remainder; ++n)
        {
            const uint8_t bitonal = *(reinterpret_cast<const uint8_t*>(bitonal_line) + n / 8);
            const uint8_t bit = 0x80 >> (n % 8);
            if (bitonal & bit)
            {
                *destination_line = *source_line;
            }

            ++source_line;
            ++destination_line;
        }
    }
}

void CopyWithBitonalMask_Gray16_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint16_t* destination,
    std::uint32_t destination_stride)
{
    const __m128i zero = _mm_setzero_si128();

    const uint32_t width_over16 = width / 16;
    const uint32_t width_remainder = width % 16;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* bitonal_line = reinterpret_cast<const uint16_t*>(source_bitonal + static_cast<size_t>(y) * source_bitonal_stride);
        const uint16_t* source_line = reinterpret_cast<const uint16_t*>(reinterpret_cast<const uint8_t*>(source) + static_cast<size_t>(y) * source_stride);
        uint16_t* destination_line = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destination_stride);

        for (uint32_t x16 = 0; x16 < width_over16; ++x16)
        {
            const __m128i value1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(source_line));
            const __m128i value2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(source_line + 8));

            // load two bytes from the bitonal source, and repeat each byte 8 times
            // so, we get bitonal = src[0] src[0] src[0] src[0] src[0] src[0] src[0] src[0] | src[1] src[1] src[1] src[1] src[1] src[1] src[1] src[1]
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*bitonal_line)), kShuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i and_with_mask = _mm_andnot_si128(bitonal, kBitSelectMask);
            const __m128i mask_for_store = _mm_cmpeq_epi8(and_with_mask, zero);

            // extend bytes to words, so 0x00 -> 0x0000 and 0xff -> 0xffff
            const __m128i mask_for_store_words_low = _mm_cvtepi8_epi16(mask_for_store);
            const __m128i mask_for_store_words_high = _mm_cvtepi8_epi16(_mm_unpackhi_epi64(mask_for_store, mask_for_store));

            _mm_maskmoveu_si128(value1, mask_for_store_words_low, reinterpret_cast<char*>(destination_line));
            _mm_maskmoveu_si128(value2, mask_for_store_words_high, reinterpret_cast<char*>(destination_line + 8));

            ++bitonal_line;
            source_line += 16;
            destination_line += 16;
        }

        // If there is a remainder, we need to do it the slow way. The remainder is always less than 16.
        for (uint32_t n = 0; n < width_remainder; ++n)
        {
            const uint8_t bitonal = *(reinterpret_cast<const uint8_t*>(bitonal_line) + n / 8);
            const uint8_t bit = 0x80 >> (n % 8);
            if (bitonal & bit)
            {
                *destination_line = *source_line;
            }

            ++source_line;
            ++destination_line;
        }
    }
}
#endif