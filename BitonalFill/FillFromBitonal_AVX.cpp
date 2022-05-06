#include "config.h"

#if BITONALFILL_HASAVX 

#include <immintrin.h>
#include <algorithm>
#include "FillFromBitonal.h"
#include "ColorPixelStructs.h"

#include "FillRemainderLine.h"

using namespace std;

static const __m128i bitSelectMask = _mm_set_epi8(0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80), 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80));
static const __m128i shuffleConstForRepeatBytes8Times = _mm_set_epi8(0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

template<typename T>
void FillFromBitonalFromZeroesOrOnes_Gray8_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes,
    const T& _op_)
{
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
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*ptrSrc)), shuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _op_(bitonal, bitSelectMask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);
            _mm_maskmoveu_si128(value, maskForStore, reinterpret_cast<char*>(ptrDst));

            ++ptrSrc;
            ptrDst += 16;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<uint8_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<uint8_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
        }
    }
}

template<typename T>
void FillFromBitonalFromZeroesOrOnes_Gray16_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes,
    const T& _op_)
{
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
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*ptrSrc)), shuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _op_(bitonal, bitSelectMask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);

            // extend bytes to words, so 0x00 -> 0x0000 and 0xff -> 0xffff
            const __m128i maskForStoreWordsLow = _mm_cvtepi8_epi16(maskForStore);
            const __m128i maskForStoreWordsHigh = _mm_cvtepi8_epi16(_mm_unpackhi_epi64(maskForStore, maskForStore));

            _mm_maskmoveu_si128(value, maskForStoreWordsLow, reinterpret_cast<char*>(ptrDst));
            _mm_maskmoveu_si128(value, maskForStoreWordsHigh, reinterpret_cast<char*>(ptrDst + 8));

            ++ptrSrc;
            ptrDst += 16;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<uint16_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<uint16_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
        }
    }
}

template <typename T>
void FillFromBitonalFromZeroesOrOnes_Bgr24_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnesRed,
    std::uint8_t valueForOnesGreen,
    std::uint8_t valueForOnesBlue,
    const T& _op_)
{
    static const __m128i shuffleConstValue1 = _mm_setr_epi8(0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00);
    static const __m128i shuffleConstValue2 = _mm_setr_epi8(0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01);
    static const __m128i shuffleConstValue3 = _mm_setr_epi8(0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02);
    static const __m128i shuffleConstMask1 = _mm_setr_epi8(0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05);
    static const __m128i shuffleConstMask2 = _mm_setr_epi8(0x05, 0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a);
    static const __m128i shuffleConstMask3 = _mm_setr_epi8(0x0a, 0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f);
    const __m128i value = _mm_set1_epi32(static_cast<int>((valueForOnesRed << 16) | (valueForOnesGreen << 8) | valueForOnesBlue));
    const __m128i value1 = _mm_shuffle_epi8(value, shuffleConstValue1);
    const __m128i value2 = _mm_shuffle_epi8(value, shuffleConstValue2);
    const __m128i value3 = _mm_shuffle_epi8(value, shuffleConstValue3);
    const __m128i zero = _mm_setzero_si128();

    const  BgrGray8 valueStruct = BgrGray8{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };

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
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*ptrSrc)), shuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _op_(bitonal, bitSelectMask);
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

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<BgrGray8>(widthRemainder, reinterpret_cast<const uint8_t*>(ptrSrc), reinterpret_cast<BgrGray8*>(ptrDst), valueStruct);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<BgrGray8>(widthRemainder, reinterpret_cast<const uint8_t*>(ptrSrc), reinterpret_cast<BgrGray8*>(ptrDst), valueStruct);
        }
    }
}

template <typename T>
void FillFromBitonalFromZeroesOrOnes_Bgr48_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnesRed,
    std::uint16_t valueForOnesGreen,
    std::uint16_t valueForOnesBlue,
    const T& _op_)
{
    static const __m128i shuffleConstValue1_1 = _mm_setr_epi8(0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x01, 0x02, 0x03);
    static const __m128i shuffleConstValue1_2 = _mm_setr_epi8(0x04, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x01);
    static const __m128i shuffleConstValue2_1 = _mm_setr_epi8(0x02, 0x03, 0x04, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05);
    static const __m128i shuffleConstMask1 = _mm_setr_epi8(0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05);
    static const __m128i shuffleConstMask2 = _mm_setr_epi8(0x05, 0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a);
    static const __m128i shuffleConstMask3 = _mm_setr_epi8(0x0a, 0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f);
    const __m128i value = _mm_set1_epi64x(((static_cast<long long>(valueForOnesRed) << 32) | (static_cast<long long>(valueForOnesGreen) << 16) | valueForOnesBlue));
    const __m128i value1_1 = _mm_shuffle_epi8(value, shuffleConstValue1_1);
    const __m128i value1_2 = _mm_shuffle_epi8(value, shuffleConstValue1_2);
    const __m128i value2_1 = _mm_shuffle_epi8(value, shuffleConstValue2_1);
    const __m128i zero = _mm_setzero_si128();

    const  BgrGray16 valueStruct = BgrGray16{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };

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
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*ptrSrc)), shuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _op_(bitonal, bitSelectMask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);

            const __m128i maskForStore1 = _mm_shuffle_epi8(maskForStore, shuffleConstMask1);
            const __m128i maskForStore2 = _mm_shuffle_epi8(maskForStore, shuffleConstMask2);
            const __m128i maskForStore3 = _mm_shuffle_epi8(maskForStore, shuffleConstMask3);
            _mm_maskmoveu_si128(value1_1, _mm_unpacklo_epi8(maskForStore1, maskForStore1), reinterpret_cast<char*>(ptrDst));
            _mm_maskmoveu_si128(value1_2, _mm_unpackhi_epi8(maskForStore1, maskForStore1), reinterpret_cast<char*>(ptrDst + 8));
            _mm_maskmoveu_si128(value2_1, _mm_unpacklo_epi8(maskForStore2, maskForStore2), reinterpret_cast<char*>(ptrDst + 16));
            _mm_maskmoveu_si128(value1_1, _mm_unpackhi_epi8(maskForStore2, maskForStore2), reinterpret_cast<char*>(ptrDst + 24));
            _mm_maskmoveu_si128(value1_2, _mm_unpacklo_epi8(maskForStore3, maskForStore3), reinterpret_cast<char*>(ptrDst + 32));
            _mm_maskmoveu_si128(value2_1, _mm_unpackhi_epi8(maskForStore3, maskForStore3), reinterpret_cast<char*>(ptrDst + 40));

            ++ptrSrc;
            ptrDst += 48;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<BgrGray16>(widthRemainder, reinterpret_cast<const uint8_t*>(ptrSrc), reinterpret_cast<BgrGray16*>(ptrDst), valueStruct);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<BgrGray16>(widthRemainder, reinterpret_cast<const uint8_t*>(ptrSrc), reinterpret_cast<BgrGray16*>(ptrDst), valueStruct);
        }
    }
}

template<typename T>
void FillFromBitonalFromZeroesOrOnes_Float32_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes,
    const T& _op_)
{
    const __m128i value = _mm_castps_si128(_mm_set_ps1(valueForOnes));
    const __m128i zero = _mm_setzero_si128();

    const uint32_t widthOver16 = width / 16;
    const uint32_t widthRemainder = width % 16;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* ptrSrc = reinterpret_cast<const uint16_t*>(sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        float* ptrDst = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destinationStride);

        for (uint32_t x16 = 0; x16 < widthOver16; ++x16)
        {
            // load two bytes from the bitonal source, and repeat each byte 8 times
            // so, we get bitonal = src[0] src[0] src[0] src[0] src[0] src[0] src[0] src[0] | src[1] src[1] src[1] src[1] src[1] src[1] src[1] src[1]
            const __m128i bitonal = _mm_shuffle_epi8(_mm_set1_epi16(static_cast<short>(*ptrSrc)), shuffleConstForRepeatBytes8Times);

            // now we and it with the mask, which selects one bit in each byte, so we have
            // src[0]&0x80 src[0]&0x40 src[0]&0x20 src[0]&0x10 src[0]&0x08 src[0]&0x04 src[0]&0x02 src[0]&0x01 | src[1]&0x80 src[1]&0x40 src[1]&0x20 src[1]&0x10 src[1]&0x08 src[1]&0x04 src[1]&0x02 src[1]&0x01
            const __m128i andWithMask = _op_(bitonal, bitSelectMask);
            const __m128i maskForStore = _mm_cmpeq_epi8(andWithMask, zero);

            // extend bytes to words, so 0x00 -> 0x0000 and 0xff -> 0xffff
            const __m128i maskForStoreWordsLow = _mm_cvtepi8_epi16(maskForStore);
            const __m128i maskForStoreWordsHigh = _mm_cvtepi8_epi16(_mm_unpackhi_epi64(maskForStore, maskForStore));

            _mm_maskmoveu_si128(value, _mm_cvtepi16_epi32(maskForStoreWordsLow), reinterpret_cast<char*>(ptrDst));
            _mm_maskmoveu_si128(value, _mm_cvtepi16_epi32(_mm_unpackhi_epi64(maskForStoreWordsLow, maskForStoreWordsLow)), reinterpret_cast<char*>(ptrDst + 4));
            _mm_maskmoveu_si128(value, _mm_cvtepi16_epi32(maskForStoreWordsHigh), reinterpret_cast<char*>(ptrDst + 8));
            _mm_maskmoveu_si128(value, _mm_cvtepi16_epi32(_mm_unpackhi_epi64(maskForStoreWordsHigh, maskForStoreWordsHigh)), reinterpret_cast<char*>(ptrDst + 12));

            ++ptrSrc;
            ptrDst += 16;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<float>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<float>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
        }
    }
}

struct AndNotOperation
{
    static constexpr bool isFromOnes = true;

    __m128i operator()(__m128i a, __m128i b) const
    {
        return _mm_andnot_si128(a, b);
    }
};

void FillFromBitonalFromOnes_Gray8_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes)
{
    const AndNotOperation andNot;
    FillFromBitonalFromZeroesOrOnes_Gray8_AVX<AndNotOperation>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnes, andNot);
}

void FillFromBitonalFromOnes_Gray16_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes)
{
    const AndNotOperation andNot;
    FillFromBitonalFromZeroesOrOnes_Gray16_AVX<AndNotOperation>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnes, andNot);
}

void FillFromBitonalFromOnes_Float32_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes)
{
    const AndNotOperation andNot;
    FillFromBitonalFromZeroesOrOnes_Float32_AVX<AndNotOperation>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnes, andNot);
}

void FillFromBitonalFromOnes_Bgr24_AVX(
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
    const AndNotOperation andNot;
    FillFromBitonalFromZeroesOrOnes_Bgr24_AVX<AndNotOperation>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnesRed, valueForOnesGreen, valueForOnesBlue, andNot);
}

void FillFromBitonalFromOnes_Bgr48_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnesRed,
    std::uint16_t valueForOnesGreen,
    std::uint16_t valueForOnesBlue)
{
    const AndNotOperation andNot;
    FillFromBitonalFromZeroesOrOnes_Bgr48_AVX<AndNotOperation>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnesRed, valueForOnesGreen, valueForOnesBlue, andNot);
}

struct AndOperator
{
    static constexpr  bool isFromOnes = false;

    __m128i operator()(__m128i a, __m128i b) const
    {
        return _mm_and_si128(a, b);
    }
};

void FillFromBitonalFromZeroes_Gray8_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForZeroes)
{
    const AndOperator andOp;
    FillFromBitonalFromZeroesOrOnes_Gray8_AVX<AndOperator>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroes, andOp);
}

void FillFromBitonalFromZeroes_Gray16_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForZeroes)
{
    const AndOperator andOp;
    FillFromBitonalFromZeroesOrOnes_Gray16_AVX<AndOperator>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroes, andOp);
}

void FillFromBitonalFromZeroes_Bgr24_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForZeroesRed,
    std::uint8_t valueForZeroesGreen,
    std::uint8_t valueForZeroesBlue)
{
    const AndOperator andOp;
    FillFromBitonalFromZeroesOrOnes_Bgr24_AVX<AndOperator>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroesRed, valueForZeroesGreen, valueForZeroesBlue, andOp);
}

void FillFromBitonalFromZeroes_Bgr48_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForZeroesRed,
    std::uint16_t valueForZeroesGreen,
    std::uint16_t valueForZeroesBlue)
{
    const AndOperator andOp;
    FillFromBitonalFromZeroesOrOnes_Bgr48_AVX<AndOperator>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroesRed, valueForZeroesGreen, valueForZeroesBlue, andOp);
}

void FillFromBitonalFromZeroes_Float32_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForZeroes)
{
    const AndOperator andOp;
    FillFromBitonalFromZeroesOrOnes_Float32_AVX<AndOperator>(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroes, andOp);
}

#endif
