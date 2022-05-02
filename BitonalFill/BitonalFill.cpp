// BitonalFill.cpp : Defines the entry point for the application.
//

#include "BitonalFill.h"
#include "config.h"

#include <chrono>
#include "FillFromBitonal.h"
#include "FillFromBitonalFillTest.h"

using namespace std;

static void Test()
{
    const uint32_t REPEAT = 100;
    const uint32_t width = 2048;
    const uint32_t height = 2048;

#if BITONALFILL_HASAVX
    static const char* SimdName = "AVX";
#elif BITONALFILL_HASNEON
    static const char* SimdName = "NEON";
#endif

    Bitmap bitonal = CreateBitmapWithRandomContent(PixelType::Bitonal, width, height);

    {
        Bitmap destGray8_C = CreateBitmapWithRandomContent(PixelType::Gray8, width, height);
        Bitmap destGray8_AVX = CreateBitmap(PixelType::Gray8, width, height);
        CopyBitmap(destGray8_AVX, destGray8_C);

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
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Gray8_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_AVX.data.get(), destGray8_AVX.stride, 0x42);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Gray8_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_AVX.data.get(), destGray8_AVX.stride, 0x42);
#endif
        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Gray8 (" << SimdName << ")" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destGray8_C, destGray8_AVX);
        cout << "Gray8: " << (b == true ? "ok" : "error") << endl;
    }

    {
        Bitmap destGray16_C = CreateBitmapWithRandomContent(PixelType::Gray16, width, height);
        Bitmap destGray16_AVX = CreateBitmap(PixelType::Gray16, width, height);
        CopyBitmap(destGray16_AVX, destGray16_C);

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
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Gray16_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destGray16_AVX.data.get(), destGray16_AVX.stride, 0x4245);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Gray16_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destGray16_AVX.data.get(), destGray16_AVX.stride, 0x4245);
#endif

        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Gray16 (" << SimdName << ")" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destGray16_C, destGray16_AVX);
        cout << "Gray16: " << (b == true ? "ok" : "error") << endl;
    }

    {
        Bitmap destBgr24_C = CreateBitmapWithRandomContent(PixelType::Bgr24, width, height);
        Bitmap destBgr24_AVX = CreateBitmap(PixelType::Bgr24, width, height);
        CopyBitmap(destBgr24_AVX, destBgr24_C);

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
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Bgr24_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destBgr24_AVX.data.get(), destBgr24_AVX.stride, 0x42, 0x44, 0x84);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Bgr24_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destBgr24_AVX.data.get(), destBgr24_AVX.stride, 0x42, 0x44, 0x84);
#endif
        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Bgr24 (" << SimdName << ")" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destBgr24_C, destBgr24_AVX);
        cout << "Bgr24: " << (b == true ? "ok" : "error") << endl;
    }

    {
        Bitmap destBgr48_C = CreateBitmapWithRandomContent(PixelType::Bgr48, width, height);
        Bitmap destBgr48_AVX = CreateBitmap(PixelType::Bgr48, width, height);
        CopyBitmap(destBgr48_AVX, destBgr48_C);

        auto start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromOnes_Bgr48_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destBgr48_C.data.get(), destBgr48_C.stride, 0x4252, 0x4454, 0x8494);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destBgr48_AVX.stride * destBgr48_AVX.height;
        cout << "Bgr48 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Bgr48_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destBgr48_AVX.data.get(), destBgr48_AVX.stride, 0x4252, 0x4454, 0x8494);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Bgr48_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destBgr48_AVX.data.get(), destBgr48_AVX.stride, 0x4252, 0x4454, 0x8494);
#endif
        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Bgr48 (" << SimdName << ")" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destBgr48_C, destBgr48_AVX);
        cout << "Bgr48: " << (b == true ? "ok" : "error") << endl;
    }

    {
        Bitmap destFloat32_C = CreateBitmapWithRandomContent(PixelType::Float32, width, height);
        Bitmap destFloat32_AVX = CreateBitmap(PixelType::Float32, width, height);
        CopyBitmap(destFloat32_AVX, destFloat32_C);

        auto start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromOnes_Float32_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)destFloat32_C.data.get(), destFloat32_C.stride, 1.2345f);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destFloat32_AVX.stride * destFloat32_AVX.height;
        cout << "Float32 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Float32_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)destFloat32_AVX.data.get(), destFloat32_AVX.stride, 1.2345f);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Float32_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)destFloat32_AVX.data.get(), destFloat32_AVX.stride, 1.2345f);
#endif

        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Float32 (" << SimdName << ")" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destFloat32_C, destFloat32_AVX);
        cout << "Float32: " << (b == true ? "ok" : "error") << endl;
    }
}

static bool TestCase(PixelType pixeltype, uint32_t width, uint32_t height)
{
    Bitmap bitonal = CreateBitmapWithRandomContent(PixelType::Bitonal, width, height);
    Bitmap dest_C = CreateBitmapWithRandomContent(pixeltype, width, height);
    Bitmap dest_Simd = CreateBitmap(pixeltype, width, height);
    CopyBitmap(dest_Simd, dest_C);
    switch (pixeltype)
    {
    case PixelType::Gray8:
        FillFromBitonalFromOnes_Gray8_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_C.data.get(), dest_C.stride, 0x48);
#if BITONALFILL_HASAVX
        FillFromBitonalFromOnes_Gray8_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48);
#elif BITONALFILL_HASNEON
        FillFromBitonalFromOnes_Gray8_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48);
#endif
        break;
    case PixelType::Gray16:
        FillFromBitonalFromOnes_Gray16_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_C.data.get(), dest_C.stride, 0x4849);
#if BITONALFILL_HASAVX
        FillFromBitonalFromOnes_Gray16_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4849);
#elif BITONALFILL_HASNEON
        FillFromBitonalFromOnes_Gray16_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4849);
#endif
        break;
    case PixelType::Bgr24:
        FillFromBitonalFromOnes_Bgr24_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_C.data.get(), dest_C.stride, 0x48, 0x49, 0x4a);
#if BITONALFILL_HASAVX
        FillFromBitonalFromOnes_Bgr24_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48, 0x49, 0x4a);
#elif BITONALFILL_HASNEON
        FillFromBitonalFromOnes_Bgr24_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48, 0x49, 0x4a);
#endif
        break;
    case PixelType::Bgr48:
        FillFromBitonalFromOnes_Bgr48_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_C.data.get(), dest_C.stride, 0x4858, 0x4959, 0x4a5a);
#if BITONALFILL_HASAVX
        FillFromBitonalFromOnes_Bgr48_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4858, 0x4959, 0x4a5a);
#elif BITONALFILL_HASNEON
        FillFromBitonalFromOnes_Bgr48_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4858, 0x4959, 0x4a5a);
#endif
        break;
    case PixelType::Float32:
        FillFromBitonalFromOnes_Float32_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_C.data.get(), dest_C.stride, 42.4f);
#if BITONALFILL_HASAVX
        FillFromBitonalFromOnes_Float32_AVX2(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_Simd.data.get(), dest_Simd.stride, 42.4f);
#elif BITONALFILL_HASNEON
        FillFromBitonalFromOnes_Float32_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_Simd.data.get(), dest_Simd.stride, 42.4f);
#endif
        break;
    }

    return Compare(dest_C, dest_Simd);
}

static void StressTest()
{
    bool b = TestCase(PixelType::Gray8, 113, 2007);
    cout << "Test 1 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCase(PixelType::Gray16, 113, 2007);
    cout << "Test 2 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCase(PixelType::Bgr24, 113, 2007);
    cout << "Test 3 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCase(PixelType::Bgr48, 113, 2007);
    cout << "Test 4 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCase(PixelType::Float32, 113, 2007);
    cout << "Test 5 -> " << (b == true ? "ok" : "error") << endl;

    b = true;
    for (uint32_t w = 183; w < 200; ++w)
    {
        b &= TestCase(PixelType::Gray8, w, 17);
        b &= TestCase(PixelType::Gray16, w, 17);
        b &= TestCase(PixelType::Bgr24, w, 17);
        b &= TestCase(PixelType::Bgr48, w, 17);
        b &= TestCase(PixelType::Float32, w, 17);
    }

    cout << "Test 6 -> " << (b == true ? "ok" : "error") << endl;
}

int main()
{
    Test();
    cout << endl << endl;
    StressTest();

    /*
    uint8_t bitonalSrc[] = { 0x88,0x44 };
    uint8_t gray8Bitmap[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

    FillFromBitonalFromOnes_Gray8_NEON(16, 1, bitonalSrc, 2, gray8Bitmap, 16, 0xff);
    //FillFromBitonalFromOnes_Gray8_AVX2(16, 1, bitonalSrc, 2, gray8Bitmap, 16, 0xff);

    uint16_t gray16Bitmap[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    //FillFromBitonalFromOnes_Gray16_AVX2(16, 1, bitonalSrc, 2, gray16Bitmap, 32, 0xffef);
    FillFromBitonalFromOnes_Gray16_NEON(16, 1, bitonalSrc, 2, gray16Bitmap, 32, 0xffef);

    uint8_t bgr24Bitmap[16 * 3] = { 1, 3, 5, 2, 3, 5, 3, 3, 5, 4, 3, 5, 5, 3, 5, 6, 3, 5, 7, 3, 5, 8, 3, 5, 9, 3, 5, 10, 3, 5, 11, 3, 5, 12, 3, 5, 13, 3, 5, 14, 3, 5, 15, 3, 5, 16, 3, 5 };
    FillFromBitonalFromOnes_Bgr24_AVX2(16, 1, bitonalSrc, 2, bgr24Bitmap, 16 * 3, 0xf1, 0xf2, 0xf3);
    uint16_t bgr48Bitmap[16 * 3] = { 1, 3, 5, 2, 3, 5, 3, 3, 5, 4, 3, 5, 5, 3, 5, 6, 3, 5, 7, 3, 5, 8, 3, 5, 9, 3, 5, 10, 3, 5, 11, 3, 5, 12, 3, 5, 13, 3, 5, 14, 3, 5, 15, 3, 5, 16, 3, 5 };
    FillFromBitonalFromOnes_Bgr48_AVX2(16, 1, bitonalSrc, 2, bgr48Bitmap, 16 * 3, 0xf1e1, 0xf2e2, 0xf3e3);

    float float32Bitmap[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    FillFromBitonalFromOnes_Float32_NEON(16, 1, bitonalSrc, 2, float32Bitmap, 16 * 4, 1.42f);
    */
    return 0;
}
