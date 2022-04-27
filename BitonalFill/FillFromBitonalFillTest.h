#pragma once

#include <cstdint>
#include <memory>

class FillFromBitonalTest
{
private:
    std::uint32_t width_;
    std::uint32_t height_;

    std::unique_ptr<std::uint8_t, decltype(&free)> bitonalBitmap;
    std::uint32_t bitonalBitmapStride;
public:
    FillFromBitonalTest(std::uint32_t width, std::uint32_t height);


private:
    static void FillWithRandomData(void* ptr, size_t size);
};