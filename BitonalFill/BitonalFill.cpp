// BitonalFill.cpp : Defines the entry point for the application.
//

#include "BitonalFill.h"
#include "FillFromBitonal.h"
#include "FillFromBitonalFillTest.h"

using namespace std;

static void Test()
{
    const uint32_t width = 2048;
    const uint32_t height = 2048;

    Bitmap bitonal = CreateBitmapWithRandomContentBitonal(width, height);
    Bitmap sourceGray8 = CreateBitmapWithRandomContentGray8(width, height);

    Bitmap destGray8_C = CreateBitmap(PixelType::Gray8, width, height);
    Bitmap destGray8_AVX = CreateBitmap(PixelType::Gray8, width, height);

    FillFromBitonalFromOnes_Gray8_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_C.data.get(), destGray8_C.stride, 0x42);
    FillFromBitonalFromOnes_Gray8_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_AVX.data.get(), destGray8_AVX.stride, 0x42);
    bool b = Compare(destGray8_C, destGray8_AVX);
    cout << "Gray8: " << (b == true ? "ok" : "error") << endl;
}

int main()
{
    Test();
    cout << "Hello CMake." << endl;

    uint8_t bitonalSrc[] = { 0x88,0x44 };
    uint8_t gray8Bitmap[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

    FillFromBitonalFromOnes_Gray8_AVX2(16, 1, bitonalSrc, 2, gray8Bitmap, 16, 0xff);

    uint16_t gray16Bitmap[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    FillFromBitonalFromOnes_Gray16_AVX2(16, 1, bitonalSrc, 2, gray16Bitmap, 32, 0xffef);

    uint8_t bgr24Bitmap[16 * 3] = { 1, 3, 5, 2, 3, 5, 3, 3, 5, 4, 3, 5, 5, 3, 5, 6, 3, 5, 7, 3, 5, 8, 3, 5, 9, 3, 5, 10, 3, 5, 11, 3, 5, 12, 3, 5, 13, 3, 5, 14, 3, 5, 15, 3, 5, 16, 3, 5 };
    FillFromBitonalFromOnes_Bgr24_AVX2(16, 1, bitonalSrc, 2, bgr24Bitmap, 16 * 3, 0xf1, 0xf2, 0xf3);

    return 0;
}
