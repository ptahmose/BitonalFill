#include "config.h"

#if BITONALFILL_HASNEON

#include <arm_neon.h>
#include <algorithm>
#include "FillFromBitonal.h"
#include "ColorPixelStructs.h"

#include "FillRemainderLine.h"

void FillFromBitonalFromOnes_Gray8_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes)
{
    uint8x8_t value = vdup_n_u8(valueForOnes);
    uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);
    //bitSelectMask.n64_u64[0] = 0x0102040810204080;

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint8_t* ptrDst = destination + static_cast<size_t>(y) * destinationStride;

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            uint8x8_t vec = vtst_u8(bitSelectMask, bitonal);
            uint8x8_t notVec = vmvn_u8(vec);
            uint8x8_t r = vorr_u8(vand_u8(vld1_u64((uint64_t*)ptrDst), notVec), vand_u8(vec, value));
            vst1_u64(ptrDst, r);

            ++ptrSrc;
            ptrDst += 8;
        }

        FillRemainderLineFromBitonalFromOnes<uint8_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
    }
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
    const uint16x8_t value = vmovq_n_u16(valueForOnes);
    uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint16_t* ptrDst = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destinationStride);

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            const uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            const uint8x8_t vec = vtst_u8(bitSelectMask, bitonal);
            const uint16x8_t vec16 = vmovl_s8(vec);
            const uint16x8_t notVec16 = vmovl_s8(vmvn_u8(vec));

            const uint16x8_t r = vorrq_u16(vandq_u16(vld1q_u16((uint64_t*)ptrDst), notVec16), vandq_u16(vec16, value));
            vst1q_u16(ptrDst, r);

            ++ptrSrc;
            ptrDst += 8;
        }

        FillRemainderLineFromBitonalFromOnes<uint16_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
    }
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
    const  BgrGray8 valueStruct = BgrGray8{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };
    uint8x8_t valueRed = vmov_n_u8(valueForOnesRed);
    uint8x8_t valueGreen = vmov_n_u8(valueForOnesGreen);
    uint8x8_t valueBlue = vmov_n_u8(valueForOnesBlue);
    uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint8_t* ptrDst = destination + static_cast<size_t>(y) * destinationStride;

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            uint8x8_t vec = vtst_u8(bitSelectMask, bitonal);
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
            ptrDst += 8*3;
        }

        FillRemainderLineFromBitonalFromOnes<BgrGray8>(widthRemainder, reinterpret_cast<const uint8_t*>(ptrSrc), reinterpret_cast<BgrGray8*>(ptrDst), valueStruct);
    }
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
    const  BgrGray16 valueStruct = BgrGray16{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };
    const uint16x8_t valueRed = vmovq_n_u16(valueForOnesRed);
    const uint16x8_t valueGreen = vmovq_n_u16(valueForOnesGreen);
    const uint16x8_t valueBlue = vmovq_n_u16(valueForOnesBlue);
    uint8x8_t bitSelectMask = vcreate_u8(0x0102040810204080ULL);

    const uint32_t widthOver8 = width / 8;
    const uint32_t widthRemainder = width % 8;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = (sourceBitonal + static_cast<size_t>(y) * sourceBitonalStride);
        uint16_t* ptrDst = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(destination) + static_cast<size_t>(y) * destinationStride);

        for (uint32_t x8 = 0; x8 < widthOver8; ++x8)
        {
            uint8x8_t bitonal = vdup_n_u8(*ptrSrc);
            uint8x8_t vec = vtst_u8(bitSelectMask, bitonal);
            uint16x8_t vec16 = vmovl_s8(vec);
            uint16x8_t notVec16 = vmovl_s8(vmvn_u8(vec));

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

        FillRemainderLineFromBitonalFromOnes<BgrGray16>(widthRemainder, reinterpret_cast<const uint8_t*>(ptrSrc), reinterpret_cast<BgrGray16*>(ptrDst), valueStruct);
    }
}

#endif



