#include "FillFromBitonal.h"
#include "ColorPixelStructs.h"

using namespace std;

template<typename T>
struct FillFromBitonalData
{
    uint32_t width;
    uint32_t height;
    const uint8_t* ptrSrc;
    uint32_t srcStride;
    T* ptrDest;
    uint32_t destStride;
    T value;
};

template <typename T>
void FillFromBitonalFromOnes(const FillFromBitonalData<T>& info)
{
    const uint32_t widthOverEight = info.width / 8;
    const uint32_t widthRemainder = info.width % 8;

    for (uint32_t y = 0; y < info.height; ++y)
    {
        T* ptrDest = reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(info.ptrDest) + static_cast<size_t>(y) * info.destStride);
        const uint8_t* ptrSrc = info.ptrSrc + static_cast<size_t>(y) * info.srcStride;

        for (uint32_t x_over_eight = 0; x_over_eight < widthOverEight; ++x_over_eight)
        {
            uint8_t v = *ptrSrc;
            uint8_t mask = 0x80;
            for (uint8_t i = 0; i < 8; ++i)
            {
                if (v & mask)
                {
                    ptrDest[i] = info.value;
                }

                mask >>= 1;
            }

            ++ptrSrc;
            ptrDest += 8;
        }

        if (widthRemainder > 0)
        {
            uint8_t v = *ptrSrc;
            uint8_t mask = 0x80;
            for (uint32_t i = 0; i < widthRemainder; ++i)
            {
                if (v & mask)
                {
                    ptrDest[i] = info.value;
                }

                mask >>= 1;
            }
        }
    }
}

void FillFromBitonalFromOnes_Gray8_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes)
{
    FillFromBitonalData<uint8_t> data;
    data.width = width;
    data.height = height;
    data.ptrSrc = sourceBitonal;
    data.srcStride = sourceBitonalStride;
    data.ptrDest = destination;
    data.destStride = destinationStride;
    data.value = valueForOnes;
    FillFromBitonalFromOnes<uint8_t>(data);
}

void FillFromBitonalFromOnes_Gray16_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes)
{
    FillFromBitonalData<uint16_t> data;
    data.width = width;
    data.height = height;
    data.ptrSrc = sourceBitonal;
    data.srcStride = sourceBitonalStride;
    data.ptrDest = destination;
    data.destStride = destinationStride;
    data.value = valueForOnes;
    FillFromBitonalFromOnes<uint16_t>(data);
}

void FillFromBitonalFromOnes_Float32_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes)
{
    FillFromBitonalData<float> data;
    data.width = width;
    data.height = height;
    data.ptrSrc = sourceBitonal;
    data.srcStride = sourceBitonalStride;
    data.ptrDest = destination;
    data.destStride = destinationStride;
    data.value = valueForOnes;
    FillFromBitonalFromOnes<float>(data);
}

void FillFromBitonalFromOnes_Bgr24_C(
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
    FillFromBitonalData<BgrGray8> data;
    data.width = width;
    data.height = height;
    data.ptrSrc = sourceBitonal;
    data.srcStride = sourceBitonalStride;
    data.ptrDest = (BgrGray8*)destination;
    data.destStride = destinationStride;
    data.value = BgrGray8{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };
    FillFromBitonalFromOnes<BgrGray8>(data);
}

void FillFromBitonalFromOnes_Bgr48_C(
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
    FillFromBitonalData<BgrGray16> data;
    data.width = width;
    data.height = height;
    data.ptrSrc = sourceBitonal;
    data.srcStride = sourceBitonalStride;
    data.ptrDest = (BgrGray16*)destination;
    data.destStride = destinationStride;
    data.value = BgrGray16{ valueForOnesBlue,valueForOnesGreen,valueForOnesRed };
    FillFromBitonalFromOnes<BgrGray16>(data);
}