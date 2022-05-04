#include "config.h"

#if BITONALFILL_HASNEON

#include <arm_neon.h>
#include <algorithm>
#include "FillFromBitonal.h"
#include "ColorPixelStructs.h"

#include "FillRemainderLine.h"

template <typename T>
void FillFromBitonalFromZeroesOrOnes_Gray8_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes,
    const T& _op_)
{
    const uint8x8_t value = vdup_n_u8(valueForOnes);
    const uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint8_t* ptrDst = destination + static_cast<size_t>(y) * destinationStride;

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            uint8x8_t vec = _op_(bitSelectMask, bitonal);
            uint8x8_t notVec = vmvn_u8(vec);
            uint8x8_t r = vorr_u8(vand_u8(vld1_u8(ptrDst), notVec), vand_u8(vec, value));
            vst1_u8(ptrDst, r);

            ++ptrSrc;
            ptrDst += 8;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<uint8_t>(widthRemainder, ptrSrc, ptrDst, valueForOnes);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<uint8_t>(widthRemainder, ptrSrc, ptrDst, valueForOnes);
        }
    }
}

template <typename T>
void FillFromBitonalFromZeroesOrOnes_Gray16_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes,
    const T& _op_)
{
    const uint16x8_t value = vmovq_n_u16(valueForOnes);
    const uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint16_t* ptrDst = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destinationStride);

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            const uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            const uint8x8_t vec = _op_(bitSelectMask, bitonal);
            const uint16x8_t vec16 = vreinterpretq_u16_s16(vmovl_s8(vreinterpret_s8_u8(vec)));
            const uint16x8_t notVec16 = vreinterpretq_u16_s16(vmovl_s8(vreinterpret_s8_u8(vmvn_u8(vec))));

            const uint16x8_t r = vorrq_u16(vandq_u16(vld1q_u16(ptrDst), notVec16), vandq_u16(vec16, value));
            vst1q_u16(ptrDst, r);

            ++ptrSrc;
            ptrDst += 8;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<uint16_t>(widthRemainder, ptrSrc, ptrDst, valueForOnes);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<uint16_t>(widthRemainder, ptrSrc, ptrDst, valueForOnes);
        }
    }
}

template <typename T>
void FillFromBitonalFromZeroesOrOnes_Bgr24_NEON(
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
    const  BgrGray8 valueStruct = BgrGray8{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };
    const uint8x8_t valueRed = vmov_n_u8(valueForOnesRed);
    const uint8x8_t valueGreen = vmov_n_u8(valueForOnesGreen);
    const uint8x8_t valueBlue = vmov_n_u8(valueForOnesBlue);
    const uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint8_t* ptrDst = destination + static_cast<size_t>(y) * destinationStride;

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            uint8x8_t vec = _op_(bitSelectMask, bitonal);
            uint8x8_t notVecu = vmvn_u8(vec);

            uint8x8x3_t m1 = vld3_u8(ptrDst);

            uint8x8_t m2_1 = vand_u8(m1.val[0], notVecu);
            uint8x8_t m2_2 = vand_u8(m1.val[1], notVecu);
            uint8x8_t m2_3 = vand_u8(m1.val[2], notVecu);

            uint8x8_t m3_1 = vand_u8(vec, valueBlue);
            uint8x8_t m3_2 = vand_u8(vec, valueGreen);
            uint8x8_t m3_3 = vand_u8(vec, valueRed);

            uint8x8_t m4_1 = vorr_u8(m2_1, m3_1);
            uint8x8_t m4_2 = vorr_u8(m2_2, m3_2);
            uint8x8_t m4_3 = vorr_u8(m2_3, m3_3);

            uint8x8x3_t m4;
            m4.val[0] = m4_1;
            m4.val[1] = m4_2;
            m4.val[2] = m4_3;

            vst3_u8(ptrDst, m4);

            ++ptrSrc;
            ptrDst += 8 * 3;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<BgrGray8>(widthRemainder, ptrSrc, reinterpret_cast<BgrGray8*>(ptrDst), valueStruct);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<BgrGray8>(widthRemainder, ptrSrc, reinterpret_cast<BgrGray8*>(ptrDst), valueStruct);
        }
    }
}

template <typename T>
void FillFromBitonalFromZeroesOrOnes_Bgr48_NEON(
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
    const  BgrGray16 valueStruct = BgrGray16{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };
    const uint16x8_t valueRed = vmovq_n_u16(valueForOnesRed);
    const uint16x8_t valueGreen = vmovq_n_u16(valueForOnesGreen);
    const uint16x8_t valueBlue = vmovq_n_u16(valueForOnesBlue);
    const uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint16_t* ptrDst = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destinationStride);

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            uint8x8_t vec = _op_(bitSelectMask, bitonal);
            uint16x8_t vec16 = vreinterpretq_u16_s16(vmovl_s8(vreinterpret_s8_u8(vec)));
            uint16x8_t notVec16 = vreinterpretq_u16_s16(vmovl_s8(vreinterpret_s8_u8(vmvn_u8(vec))));

            uint16x8x3_t m1 = vld3q_u16(ptrDst);

            uint16x8_t m2_1 = vandq_u16(m1.val[0], notVec16);
            uint16x8_t m2_2 = vandq_u16(m1.val[1], notVec16);
            uint16x8_t m2_3 = vandq_u16(m1.val[2], notVec16);

            uint16x8_t m3_1 = vandq_u16(vec16, valueBlue);
            uint16x8_t m3_2 = vandq_u16(vec16, valueGreen);
            uint16x8_t m3_3 = vandq_u16(vec16, valueRed);

            uint16x8_t m4_1 = vorrq_u16(m2_1, m3_1);
            uint16x8_t m4_2 = vorrq_u16(m2_2, m3_2);
            uint16x8_t m4_3 = vorrq_u16(m2_3, m3_3);

            uint16x8x3_t m4;
            m4.val[0] = m4_1;
            m4.val[1] = m4_2;
            m4.val[2] = m4_3;

            vst3q_u16(ptrDst, m4);

            ++ptrSrc;
            ptrDst += 8 * 3;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<BgrGray16>(widthRemainder, ptrSrc, reinterpret_cast<BgrGray16*>(ptrDst), valueStruct);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<BgrGray16>(widthRemainder, ptrSrc, reinterpret_cast<BgrGray16*>(ptrDst), valueStruct);
        }
    }
}

template <typename T>
void FillFromBitonalFromZeroesOrOnes_Float32_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes,
    const T& _op_)
{
    const uint32x4_t value = vreinterpretq_u32_f32(vmovq_n_f32(valueForOnes));
    const uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        float* ptrDst = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destinationStride);

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            const uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            const uint8x8_t vec = _op_(bitSelectMask, bitonal);

            const int16x8_t vec16 = vmovl_s8(vreinterpret_s8_u8(vec));
            const int16x8_t notVec16 = vmovl_s8(vreinterpret_s8_u8(vmvn_u8(vec)));
            const uint32x4_t vec32Lo = vreinterpretq_u32_s32(vmovl_s16(vget_low_s16(vec16)));
            const uint32x4_t vec32Hi = vreinterpretq_u32_s32(vmovl_s16(vget_high_s16(vec16)));
            const uint32x4_t notVec32Lo = vreinterpretq_u32_s32(vmovl_s16(vget_low_s16(notVec16)));
            const uint32x4_t notVec32Hi = vreinterpretq_u32_s32(vmovl_s16(vget_high_s16(notVec16)));

            uint32x4_t r = vorrq_u32(vandq_u32(vld1q_u32(reinterpret_cast<const uint32_t*>(ptrDst)), notVec32Lo), vandq_u32(vec32Lo, value));
            vst1q_u32(reinterpret_cast<uint32_t*>(ptrDst), r);
            ptrDst += 4;
            r = vorrq_u32(vandq_u32(vld1q_u32(reinterpret_cast<const uint32_t*>(ptrDst)), notVec32Hi), vandq_u32(vec32Hi, value));
            vst1q_u32(reinterpret_cast<uint32_t*>(ptrDst), r);

            ++ptrSrc;
            ptrDst += 4;
        }

        if (T::isFromOnes)
        {
            FillRemainderLineFromBitonalFromOnes<float>(widthRemainder, ptrSrc, ptrDst, valueForOnes);
        }
        else
        {
            FillRemainderLineFromBitonalFromZeroes<float>(widthRemainder, ptrSrc, ptrDst, valueForOnes);
        }
    }
}

struct FromOnes
{
    static constexpr  bool isFromOnes = true;

    uint8x8_t operator()(uint8x8_t a, uint8x8_t b) const
    {
        return vtst_u8(a, b);
    }
};

void FillFromBitonalFromOnes_Gray8_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes)
{
    const FromOnes fromOnes;
    FillFromBitonalFromZeroesOrOnes_Gray8_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnes, fromOnes);
}

void FillFromBitonalFromOnes_Gray16_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes)
{
    const FromOnes fromOnes;
    FillFromBitonalFromZeroesOrOnes_Gray16_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnes, fromOnes);
}

void FillFromBitonalFromOnes_Bgr24_NEON(
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
    const FromOnes fromOnes;
    FillFromBitonalFromZeroesOrOnes_Bgr24_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnesRed, valueForOnesGreen, valueForOnesBlue, fromOnes);
}

void FillFromBitonalFromOnes_Bgr48_NEON(
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
    const FromOnes fromOnes;
    FillFromBitonalFromZeroesOrOnes_Bgr48_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnesRed, valueForOnesGreen, valueForOnesBlue, fromOnes);
}

void FillFromBitonalFromOnes_Float32_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes)
{
    const FromOnes fromOnes;
    FillFromBitonalFromZeroesOrOnes_Float32_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForOnes, fromOnes);
}

///////////////////////////////////////////////////////////////////////////////////

struct FromZeroes
{
    static constexpr  bool isFromOnes = false;

    uint8x8_t operator()(uint8x8_t a, uint8x8_t b) const
    {
        return vmvn_u8(vtst_u8(a, b));
    }
};


void FillFromBitonalFromZeroes_Gray8_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForZeroes)
{
    FromZeroes fromZeroes;
    FillFromBitonalFromZeroesOrOnes_Gray8_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroes, fromZeroes);
}

void FillFromBitonalFromZeroes_Gray16_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForZeroes)
{
    FromZeroes fromZeroes;
    FillFromBitonalFromZeroesOrOnes_Gray16_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroes, fromZeroes);
}

void FillFromBitonalFromZeroes_Bgr24_NEON(
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
    FromZeroes fromZeroes;
    FillFromBitonalFromZeroesOrOnes_Bgr24_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroesRed, valueForZeroesGreen, valueForZeroesBlue, fromZeroes);
}

void FillFromBitonalFromZeroes_Bgr48_NEON(
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
    const FromZeroes fromZeroes;
    FillFromBitonalFromZeroesOrOnes_Bgr48_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroesRed, valueForZeroesGreen, valueForZeroesBlue, fromZeroes);
}

void FillFromBitonalFromZeroes_Float32_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForZeroes)
{
    const FromZeroes fromZeroes;
    FillFromBitonalFromZeroesOrOnes_Float32_NEON(width, height, sourceBitonal, sourceBitonalStride, destination, destinationStride, valueForZeroes, fromZeroes);
}

#endif
