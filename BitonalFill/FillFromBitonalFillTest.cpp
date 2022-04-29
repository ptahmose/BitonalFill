#include "FillFromBitonalFillTest.h"

#include <random>

using namespace std;

static void FillWithRandomData(void* ptr, size_t size)
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

Bitmap CreateBitmap(PixelType pixeltype, std::uint32_t width, std::uint32_t height)
{
    Bitmap bitmap;
    switch (pixeltype)
    {
    case PixelType::Gray8:
        bitmap.stride = width;
        break;
    case PixelType::Gray16:
        bitmap.stride = width * 2;
        break;
    case PixelType::Bgr24:
        bitmap.stride = width * 3;
        break;
    case PixelType::Bgr48:
        bitmap.stride = width * 6;
        break;
    case PixelType::Bitonal:
        bitmap.stride = (width + 7) / 8;
        break;
    default:
        throw exception();
    }

    bitmap.pixeltype = pixeltype;
    bitmap.width = width;
    bitmap.height = height;
    size_t size = static_cast<size_t>(bitmap.stride) * height;
    bitmap.data = shared_ptr<void>(malloc(size), free);
    memset(bitmap.data.get(), 88, size);
    return bitmap;
}

Bitmap CreateBitmapWithRandomContentBitonal(std::uint32_t width, std::uint32_t height)
{
    Bitmap bitmap;
    bitmap.pixeltype = PixelType::Bitonal;
    bitmap.width = width;
    bitmap.height = height;
    bitmap.stride = (width + 7) / 8;
    size_t size = static_cast<size_t>(bitmap.stride) * height;
    bitmap.data = shared_ptr<void>(malloc(size), free);
    FillWithRandomData(bitmap.data.get(), size);
    return bitmap;
}

Bitmap CreateBitmapWithRandomContentGray8(std::uint32_t width, std::uint32_t height)
{
    Bitmap bitmap;
    bitmap.pixeltype = PixelType::Gray8;
    bitmap.width = width;
    bitmap.height = height;
    bitmap.stride = width;
    size_t size = static_cast<size_t>(bitmap.stride) * height;
    bitmap.data = shared_ptr<void>(malloc(size), free);
    FillWithRandomData(bitmap.data.get(), size);
    return bitmap;
}

Bitmap CreateBitmapWithRandomContentGray16(std::uint32_t width, std::uint32_t height)
{
    Bitmap bitmap;
    bitmap.pixeltype = PixelType::Gray16;
    bitmap.width = width;
    bitmap.height = height;
    bitmap.stride = width * 2;
    size_t size = static_cast<size_t>(bitmap.stride) * height;
    bitmap.data = shared_ptr<void>(malloc(size), free);
    FillWithRandomData(bitmap.data.get(), size);
    return bitmap;
}

Bitmap CreateBitmapWithRandomContentBgr24(std::uint32_t width, std::uint32_t height)
{
    Bitmap bitmap;
    bitmap.pixeltype = PixelType::Bgr24;
    bitmap.width = width;
    bitmap.height = height;
    bitmap.stride = width * 3;
    size_t size = static_cast<size_t>(bitmap.stride) * height;
    bitmap.data = shared_ptr<void>(malloc(size), free);
    FillWithRandomData(bitmap.data.get(), size);
    return bitmap;
}

bool Compare(const Bitmap& a, const Bitmap& b)
{
    if (a.pixeltype != b.pixeltype ||
        a.width != b.width ||
        a.height != b.height)
    {
        return false;
    }

    uint8_t bytesPerPel;
    switch (a.pixeltype)
    {
    case PixelType::Gray8:
        bytesPerPel = 1;
        break;
    case PixelType::Gray16:
        bytesPerPel = 2;
        break;
    case PixelType::Bgr24:
        bytesPerPel = 3;
        break;
    case PixelType::Bgr48:
        bytesPerPel = 6;
        break;
    default:
        return false;
    }

    size_t lineLength = bytesPerPel * (size_t)a.width;
    for (uint32_t y = 0; y < a.height; ++y)
    {
        const void* lineA = (const uint8_t*)(a.data.get()) + y * a.stride;
        const void* lineB = (const uint8_t*)(b.data.get()) + y * b.stride;
        int r = memcmp(lineA, lineB, lineLength);
        if (r != 0)
        {
            return false;
        }
    }

    return true;
}

//BitmapInfo CreateBitmapWithRandomContentGray8(std::uint32_t width, std::uint32_t height);
//
//FillFromBitonalTest::FillFromBitonalTest(std::uint32_t width, std::uint32_t height) :
//    width_(width), height_(height), bitonalBitmap({}, {})
//{
//    this->bitonalBitmapStride = ((width + 7) / 8) * 8;
//    this->bitonalBitmap = unique_ptr<std::uint8_t, decltype(&free)>(
//        static_cast<uint8_t*>(malloc(static_cast<size_t>(this->bitonalBitmapStride) * height)),
//        free);
//
//    FillFromBitonalTest::FillWithRandomData(this->bitonalBitmap.get(), static_cast<size_t>(this->bitonalBitmapStride) * height);
//}
//
///*static*/void FillFromBitonalTest::FillWithRandomData(void* ptr, size_t size)
//{
//    std::random_device rd;
//    std::mt19937 gen(rd()); //seed for rd(Mersenne twister)
//    uniform_int_distribution<uint32_t> dist(0, 0xffffffff);
//    size_t sizeOverfFour = size / 4;
//    size_t remainder = size % 4;
//    for (size_t i = 0; i < sizeOverfFour; ++i)
//    {
//        ((uint32_t*)ptr)[i] = dist(gen);
//    }
//
//    if (remainder > 0)
//    {
//        uint32_t randomVal = dist(gen);
//        for (size_t i = sizeOverfFour * 4; i < size; ++i)
//        {
//            ((uint8_t*)ptr)[i] = (uint8_t)randomVal;
//            randomVal >>= 8;
//        }
//    }
//}