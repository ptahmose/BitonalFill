﻿// BitonalFill.cpp : Defines the entry point for the application.
//

#include "BitonalFill.h"

#include <chrono>

#include "FillFromBitonal.h"
#include "FillFromBitonalFillTest.h"

using namespace std;

static void Test()
{
    const uint32_t REPEAT = 100;
    const uint32_t width = 2048;
    const uint32_t height = 2048;

    Bitmap bitonal = CreateBitmapWithRandomContentBitonal(width, height);

    {
        Bitmap sourceGray8 = CreateBitmapWithRandomContentGray8(width, height);
        Bitmap destGray8_C = CreateBitmap(PixelType::Gray8, width, height);
        Bitmap destGray8_AVX = CreateBitmap(PixelType::Gray8, width, height);

        auto start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromOnes_Gray8_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_C.data.get(), destGray8_C.stride, 0x42);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destGray8_AVX.stride * destGray8_AVX.height;
        cout << "Gray8 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;


        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromOnes_Gray8_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_AVX.data.get(), destGray8_AVX.stride, 0x42);
        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Gray8 (AVX)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destGray8_C, destGray8_AVX);
        cout << "Gray8: " << (b == true ? "ok" : "error") << endl;
    }

    {
        Bitmap sourceGray16 = CreateBitmapWithRandomContentGray16(width, height);
        Bitmap destGray16_C = CreateBitmap(PixelType::Gray16, width, height);
        Bitmap destGray16_AVX = CreateBitmap(PixelType::Gray16, width, height);

        auto start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromOnes_Gray16_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destGray16_C.data.get(), destGray16_C.stride, 0x4245);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destGray16_AVX.stride * destGray16_AVX.height;
        cout << "Gray16 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromOnes_Gray16_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destGray16_AVX.data.get(), destGray16_AVX.stride, 0x4245);
        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Gray16 (AVX)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destGray16_C, destGray16_AVX);
        cout << "Gray16: " << (b == true ? "ok" : "error") << endl;
    }

    {
        Bitmap sourceBgr24 = CreateBitmapWithRandomContentBgr24(width, height);
        Bitmap destBgr24_C = CreateBitmap(PixelType::Bgr24, width, height);
        Bitmap destBgr24_AVX = CreateBitmap(PixelType::Bgr24, width, height);

        auto start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromOnes_Bgr24_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destBgr24_C.data.get(), destBgr24_C.stride, 0x42, 0x44, 0x84);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destBgr24_AVX.stride * destBgr24_AVX.height;
        cout << "Bgr24 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromOnes_Bgr24_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destBgr24_AVX.data.get(), destBgr24_AVX.stride, 0x42, 0x44, 0x84);
        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Bgr24 (AVX)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destBgr24_C, destBgr24_AVX);
        cout << "Bgr24: " << (b == true ? "ok" : "error") << endl;
    }
}

int main()
{
    Test();

    /*
    cout << "Hello CMake." << endl;

    uint8_t bitonalSrc[] = { 0x88,0x44 };
    uint8_t gray8Bitmap[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

    FillFromBitonalFromOnes_Gray8_AVX2(16, 1, bitonalSrc, 2, gray8Bitmap, 16, 0xff);

    uint16_t gray16Bitmap[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    FillFromBitonalFromOnes_Gray16_AVX2(16, 1, bitonalSrc, 2, gray16Bitmap, 32, 0xffef);

    uint8_t bgr24Bitmap[16 * 3] = { 1, 3, 5, 2, 3, 5, 3, 3, 5, 4, 3, 5, 5, 3, 5, 6, 3, 5, 7, 3, 5, 8, 3, 5, 9, 3, 5, 10, 3, 5, 11, 3, 5, 12, 3, 5, 13, 3, 5, 14, 3, 5, 15, 3, 5, 16, 3, 5 };
    FillFromBitonalFromOnes_Bgr24_AVX2(16, 1, bitonalSrc, 2, bgr24Bitmap, 16 * 3, 0xf1, 0xf2, 0xf3);
    */

    return 0;
}
