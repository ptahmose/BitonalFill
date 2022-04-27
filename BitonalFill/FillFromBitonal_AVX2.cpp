#include <immintrin.h>
#include <algorithm>
#include "FillFromBitonal.h"
#include "ColorPixelStructs.h"

using namespace std;

template <typename T>
void FillRemainderLineFromBitonalFromOnes(uint32_t count, const uint8_t* source, T* destination, T value)
{
    if (count > 0)
    {
        uint8_t v = *source;
        uint8_t mask = 0x80;
        for (uint32_t i = 0; i < max(8u, count); ++i)
        {
            if (v & mask)
            {
                destination[i] = value;
            }

            mask >>= 1;
        }

        if (count > 8)
        {
            destination += 8;
            v = *(1 + source);

            for (uint32_t i = 0; i < max(8u, count - 8); ++i)
            {
                if (v & mask)
                {
                    destination[i] = value;
                }

                mask >>= 1;
            }
        }
    }
}

void FillFromBitonalFromOnes_Gray8_AVX2(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes)
{
    static const __m128i mask = _mm_set_epi8(0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80), 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80));
    static const __m128i shuffleConst = _mm_set_epi8(0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    const __m128i value = _mm_set1_epi8(static_cast<char>(valueForOnes));
    const __m128i zero = _mm_setzero_si128();

    const uint32_t widthOver16 = width / 16;
    const uint32_t widthRemainder = width % 16;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* ptrSrc = reinterpret_cast<const uint16_t*>(sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint8_t* ptrDst = destination + static_cast<size_t>(y) * destinationStride;

        for (uint32_t x16 = 0; x16 < widthOver16; ++x16)
        {
            // load two bytes from the bitonal source, and repeat each byte 8 times
            // so, we get bitonal = src[0] src[0] src[0] src[0] src[0] src[0] src[0] src[0] | src[1] src[1] src[1] src[1] src[1] src[1] src[1] src[1]
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*ptrSrc)), shuffleConst);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _mm_andnot_si128(bitonal, mask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);
            _mm_maskmoveu_si128(value, maskForStore, reinterpret_cast<char*>(ptrDst));

            ++ptrSrc;
            ptrDst += 16;
        }

        FillRemainderLineFromBitonalFromOnes<uint8_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
    }
}

void FillFromBitonalFromOnes_Gray16_AVX2(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes)
{
    static const __m128i mask = _mm_set_epi8(0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80), 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80));
    static const __m128i shuffleConst = _mm_set_epi8(0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    const __m128i value = _mm_set1_epi16(static_cast<short>(valueForOnes));
    const __m128i zero = _mm_setzero_si128();

    const uint32_t widthOver16 = width / 16;
    const uint32_t widthRemainder = width % 16;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* ptrSrc = reinterpret_cast<const uint16_t*>(sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint16_t* ptrDst = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destinationStride);

        for (uint32_t x16 = 0; x16 < widthOver16; ++x16)
        {
            // load two bytes from the bitonal source, and repeat each byte 8 times
            // so, we get bitonal = src[0] src[0] src[0] src[0] src[0] src[0] src[0] src[0] | src[1] src[1] src[1] src[1] src[1] src[1] src[1] src[1]
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*ptrSrc)), shuffleConst);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _mm_andnot_si128(bitonal, mask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);

            // extend bytes to words, so 0x00 -> 0x0000 and 0xff -> 0xffff
            const __m128i maskForStoreWordsLow = _mm_cvtepi8_epi16(maskForStore);
            const __m128i maskForStoreWordsHigh = _mm_cvtepi8_epi16(_mm_unpackhi_epi64(maskForStore, maskForStore));

            _mm_maskmoveu_si128(value, maskForStoreWordsLow, reinterpret_cast<char*>(ptrDst));
            _mm_maskmoveu_si128(value, maskForStoreWordsHigh, reinterpret_cast<char*>(ptrDst + 8));

            ++ptrSrc;
            ptrDst += 16;
        }

        FillRemainderLineFromBitonalFromOnes<uint16_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
    }
}

void FillFromBitonalFromOnes_Bgr24_AVX2(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnesRed,
    std::uint8_t valueForOnesGreen,
    std::uint8_t valueForOnesBlue)
{
    static const __m128i mask = _mm_set_epi8(0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80), 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80));
    static const __m128i shuffleConstValue1 = _mm_setr_epi8(0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00);
    static const __m128i shuffleConstValue2 = _mm_setr_epi8(0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01);
    static const __m128i shuffleConstValue3 = _mm_setr_epi8(0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02);
    static const __m128i shuffleConst = _mm_set_epi8(0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    static const __m128i shuffleConstMask1 = _mm_setr_epi8(0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05);
    static const __m128i shuffleConstMask2 = _mm_setr_epi8(0x05, 0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a);
    static const __m128i shuffleConstMask3 = _mm_setr_epi8(0x0a, 0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f);
    const __m128i value = _mm_set1_epi32(static_cast<int>((valueForOnesRed << 16) | (valueForOnesGreen << 8) | valueForOnesBlue));
    __m128i value1 = _mm_shuffle_epi8(value, shuffleConstValue1);
    __m128i value2 = _mm_shuffle_epi8(value, shuffleConstValue2);
    __m128i value3 = _mm_shuffle_epi8(value, shuffleConstValue3);
    const __m128i zero = _mm_setzero_si128();

    const  BgrGray8 valuestruct = BgrGray8{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };

    const uint32_t widthOver16 = width / 16;
    const uint32_t widthRemainder = width % 16;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* ptrSrc = reinterpret_cast<const uint16_t*>(sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint8_t* ptrDst = destination + static_cast<size_t>(y) * destinationStride;

        for (uint32_t x16 = 0; x16 < widthOver16; ++x16)
        {
            // load two bytes from the bitonal source, and repeat each byte 8 times
            // so, we get bitonal = src[0] src[0] src[0] src[0] src[0] src[0] src[0] src[0] | src[1] src[1] src[1] src[1] src[1] src[1] src[1] src[1]
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*ptrSrc)), shuffleConst);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _mm_andnot_si128(bitonal, mask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);

            const __m128i maskForStore1 = _mm_shuffle_epi8(maskForStore, shuffleConstMask1);
            const __m128i maskForStore2 = _mm_shuffle_epi8(maskForStore, shuffleConstMask2);
            const __m128i maskForStore3 = _mm_shuffle_epi8(maskForStore, shuffleConstMask3);
            _mm_maskmoveu_si128(value1, maskForStore1, reinterpret_cast<char*>(ptrDst));
            _mm_maskmoveu_si128(value2, maskForStore2, reinterpret_cast<char*>(ptrDst + 16));
            _mm_maskmoveu_si128(value3, maskForStore3, reinterpret_cast<char*>(ptrDst + 32));

            ++ptrSrc;
            ptrDst += 48;
        }

        FillRemainderLineFromBitonalFromOnes<BgrGray8>(widthRemainder, reinterpret_cast<const uint8_t*>(ptrSrc), reinterpret_cast<BgrGray8*>(ptrDst), valuestruct);
    }
}