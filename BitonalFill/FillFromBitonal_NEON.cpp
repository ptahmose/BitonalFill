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
    uint8x8_t bitSelectMask;
    bitSelectMask.n64_u64[0] = 0x0102040810204080;

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

            //uint8x8_t l2 = vand_u8(vld1_u64(ptrDst), notVec);
            uint8x8_t r = vorr_u8(vand_u8(vld1_u64(ptrDst), notVec), vand_u8(vec, value));
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
    uint16x8_t value = vmovq_n_u16(valueForOnes);
    uint8x8_t bitSelectMask;
    bitSelectMask.n64_u64[0] = 0x0102040810204080;

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
            uint16x8_t vecu16 = vmovl_s8(vec);
            uint16x8_t notVecu16 = vmovl_s8(vmvn_u8(vec));

            uint16x8_t m1 = vld1q_u16(ptrDst);
            uint16x8_t m2 = vandq_u16(m1, notVecu16);

            uint16x8_t m3 = vandq_u16(vecu16, value);
            uint16x8_t m4 = vorrq_u16(m2, m3);
            vst1q_u16(ptrDst, m4);

            ++ptrSrc;
            ptrDst += 8;
        }

        FillRemainderLineFromBitonalFromOnes<uint16_t>(widthRemainder, (const uint8_t*)ptrSrc, ptrDst, valueForOnes);
    }
}

#endif



