#include "FillFromBitonalFillTest.h"

#include <random>

using namespace std;

FillFromBitonalTest::FillFromBitonalTest(std::uint32_t width, std::uint32_t height) :
    width_(width), height_(height), bitonalBitmap({},{})
{
    this->bitonalBitmapStride = ((width + 7) / 8) * 8;
    this->bitonalBitmap = unique_ptr<std::uint8_t, decltype(&free)>(
        static_cast<uint8_t*>(malloc(static_cast<size_t>(this->bitonalBitmapStride) * height)),
        free);

    FillFromBitonalTest::FillWithRandomData(this->bitonalBitmap.get(), static_cast<size_t>(this->bitonalBitmapStride) * height);
}

/*static*/void FillFromBitonalTest::FillWithRandomData(void* ptr, size_t size)
{
    std::random_device rd;
    std::mt19937 gen(rd()); //seed for rd(Mersenne twister)
    uniform_int_distribution<uint32_t> dist(0, 0xffffffff);
    size_t sizeOverfFour = size / 4;
    size_t remainder = size % 4;
    for (size_t i = 0; i < sizeOverfFour; ++i)
    {
        ((uint32_t*)ptr)[i] = dist(gen);
    }

    if (remainder > 0)
    {
        uint32_t randomVal = dist(gen);
        for (size_t i = sizeOverfFour * 4; i < size; ++i)
        {
            ((uint8_t*)ptr)[i] = (uint8_t)randomVal;
            randomVal >>= 8;
        }
    }
}