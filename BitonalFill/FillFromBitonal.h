#pragma once

#include <cstdint>

#include "config.h"

void FillFromBitonalFromOnes_Gray8_C(
    std::uint32_t width, 
    std::uint32_t height, 
    const std::uint8_t* sourceBitonal, 
    std::uint32_t sourceBitonalStride, 
    std::uint8_t* destination, 
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes);

void FillFromBitonalFromOnes_Gray16_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes);

void FillFromBitonalFromOnes_Bgr24_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnesRed,
    std::uint8_t valueForOnesGreen,
    std::uint8_t valueForOnesBlue);

void FillFromBitonalFromOnes_Bgr48_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnesRed,
    std::uint16_t valueForOnesGreen,
    std::uint16_t valueForOnesBlue);

void FillFromBitonalFromOnes_Float32_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes);

void FillFromBitonalFromZeroes_Gray8_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForZeroes);

void FillFromBitonalFromZeroes_Gray16_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForZeroes);

void FillFromBitonalFromZeroes_Bgr24_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForZeroesRed,
    std::uint8_t valueForZeroesGreen,
    std::uint8_t valueForZeroesBlue);

void FillFromBitonalFromZeroes_Bgr48_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForZeroesRed,
    std::uint16_t valueForZeroesGreen,
    std::uint16_t valueForZeroesBlue);

void FillFromBitonalFromZeroes_Float32_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForZeroes);

#if BITONALFILL_HASAVX
void FillFromBitonalFromOnes_Gray8_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes);

void FillFromBitonalFromOnes_Gray16_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes);

void FillFromBitonalFromOnes_Bgr24_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnesRed,
    std::uint8_t valueForOnesGreen,
    std::uint8_t valueForOnesBlue);

void FillFromBitonalFromOnes_Bgr48_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnesRed,
    std::uint16_t valueForOnesGreen,
    std::uint16_t valueForOnesBlue);

void FillFromBitonalFromOnes_Float32_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes);

void FillFromBitonalFromZeroes_Gray8_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForZeroes);

void FillFromBitonalFromZeroes_Gray16_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForZeroes);

void FillFromBitonalFromZeroes_Bgr24_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForZeroesRed,
    std::uint8_t valueForZeroesGreen,
    std::uint8_t valueForZeroesBlue);

void FillFromBitonalFromZeroes_Bgr48_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForZeroesRed,
    std::uint16_t valueForZeroesGreen,
    std::uint16_t valueForZeroesBlue);

void FillFromBitonalFromZeroes_Float32_AVX(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForZeroes);
#endif

#if BITONALFILL_HASNEON
void FillFromBitonalFromOnes_Gray8_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes);

void FillFromBitonalFromOnes_Gray16_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes);

void FillFromBitonalFromOnes_Bgr24_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnesRed,
    std::uint8_t valueForOnesGreen,
    std::uint8_t valueForOnesBlue);

void FillFromBitonalFromOnes_Bgr48_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnesRed,
    std::uint16_t valueForOnesGreen,
    std::uint16_t valueForOnesBlue);

void FillFromBitonalFromOnes_Float32_NEON(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes);
#endif
