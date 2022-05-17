#include "FillFromBitonalFillTest.h"

#include <random>
#include <cstdlib>
#include <stdexcept>
#include <cstring>

using namespace std;

size_t Bitmap::GetLineLength() const
{
    switch (pixeltype)
    {
    case PixelType::Gray8:
        return this->width;
    case PixelType::Gray16:
        return this->width * 2;
    case PixelType::Bgr24:
        return this->width * 3;
    case PixelType::Bgr48:
        return this->width * 6;
    case PixelType::Bitonal:
        return (width + 7) / 8;
    case PixelType::Float32:
        return width * 4;
    default:
        throw exception();
    }
}

void CopyBitmap(Bitmap& dst, const Bitmap& src)
{
    if (dst.height != src.height || dst.width != src.width || dst.pixeltype != src.pixeltype)
    {
        throw invalid_argument("Bitmaps must be same size and pixeltype");
    }

    const size_t lineLength = dst.GetLineLength();
    for (uint32_t y = 0; y < dst.height; ++y)
    {
        const void* lineSrc = static_cast<const uint8_t*>(src.data.get()) + y * src.stride;
        void* lineDst = static_cast<uint8_t*>(dst.data.get()) + y * dst.stride;
        memcpy(lineDst, lineSrc, lineLength);
    }
}

static void FillWithRandomData(void* ptr, size_t size, mt19937& rndGenerator)
{
    uniform_int_distribution<uint32_t> dist(0, 0xffffffff);
    size_t sizeOverfFour = size / 4;
    size_t remainder = size % 4;
    for (size_t i = 0; i < sizeOverfFour; ++i)
    {
        static_cast<uint32_t*>(ptr)[i] = dist(rndGenerator);
    }

    if (remainder > 0)
    {
        uint32_t randomVal = dist(rndGenerator);
        for (size_t i = sizeOverfFour * 4; i < size; ++i)
        {
            static_cast<uint8_t*>(ptr)[i] = static_cast<uint8_t>(randomVal);
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
        bitmap.stride = ((width * 2 + 3) / 4) * 4;
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
    case PixelType::Float32:
        bitmap.stride = width * 4;
        break;
    default:
        throw exception();
    }

    bitmap.pixeltype = pixeltype;
    bitmap.width = width;
    bitmap.height = height;
    size_t size = static_cast<size_t>(bitmap.stride) * height;
    bitmap.data = shared_ptr<void>(malloc(size), free);
    return bitmap;
}

static std::mt19937 RandomGenerator;
static bool RandomGeneratorInitialized = false;

Bitmap CreateBitmapWithRandomContent(PixelType pixeltype, std::uint32_t width, std::uint32_t height)
{
    if (RandomGeneratorInitialized == false)
    {
        RandomGenerator = std::mt19937(0x12345);
        /*std::random_device rd;
        RandomGenerator = std::mt19937(rd());*/
        RandomGeneratorInitialized = true;
    }

    Bitmap bitmap = CreateBitmap(pixeltype, width, height);
    FillWithRandomData(bitmap.data.get(), bitmap.GetTotalSize(), RandomGenerator);
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
    case PixelType::Float32:
        bytesPerPel = 4;
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
