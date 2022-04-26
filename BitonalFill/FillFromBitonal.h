#pragma once

#include <cstdint>

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

void FillFromBitonalFromOnes_Float32_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    float* destination,
    std::uint32_t destinationStride,
    float valueForOnes);

void FillFromBitonalFromOnes_Bgr8_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnesRed,
    std::uint8_t valueForOnesGreen,
    std::uint8_t valueForOnesBlue);

void FillFromBitonalFromOnes_Gray8_AVX2(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes);

void FillFromBitonalFromOnes_Gray16_AVX2(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint16_t* destination,
    std::uint32_t destinationStride,
    std::uint16_t valueForOnes);

