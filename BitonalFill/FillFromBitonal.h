#pragma once

#include <cstdint>

void FillFromBitonal_Gray8(std::uint32_t width, std::uint32_t height, const std::uint8_t* sourceBitonal, std::uint32_t sourceBitonalStride, std::uint8_t* destination, std::uint32_t destinationStride);