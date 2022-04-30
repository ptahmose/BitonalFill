#pragma once
#pragma once

#include <cstdint>
#include <memory>

enum class PixelType
{
    Bitonal,
    Gray8,
    Gray16,
    Bgr24,
    Bgr48
};

struct Bitmap
{
    PixelType pixeltype;
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t stride;
    std::shared_ptr<void> data;

    size_t GetTotalSize() const { return (size_t)this->stride * this->height; }
    size_t GetLineLength() const;
};

Bitmap CreateBitmap(PixelType pixeltype, std::uint32_t width, std::uint32_t height);
Bitmap CreateBitmapWithRandomContent(PixelType pixeltype, std::uint32_t width, std::uint32_t height);
void CopyBitmap(Bitmap& dst, const Bitmap& src);
//Bitmap CreateBitmapWithRandomContentBitonal(std::uint32_t width, std::uint32_t height);
//Bitmap CreateBitmapWithRandomContentGray8(std::uint32_t width, std::uint32_t height);
//Bitmap CreateBitmapWithRandomContentGray16(std::uint32_t width, std::uint32_t height);
//Bitmap CreateBitmapWithRandomContentBgr24(std::uint32_t width, std::uint32_t height);

bool Compare(const Bitmap& a, const Bitmap& b);

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