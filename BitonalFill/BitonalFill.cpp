// BitonalFill.cpp : Defines the entry point for the application.
//

#include "BitonalFill.h"
#include "config.h"

#include <chrono>
#include "FillFromBitonal.h"
#include "FillFromBitonalFillTest.h"
#include "CopyWithBitonalMask.h"

using namespace std;

static void TestFromBitonalZeroes()
{
    const uint32_t REPEAT = 100;
    const uint32_t width = 2048;
    const uint32_t height = 2048;

#if BITONALFILL_HASAVX
    static const char* SimdName = "AVX";
#elif BITONALFILL_HASNEON
    static const char* SimdName = "NEON";
#endif

    cout << "FillFromZeroes:" << endl;
    cout << "===============" << endl;

    Bitmap bitonal = CreateBitmapWithRandomContent(PixelType::Bitonal, width, height);

    {
        Bitmap destGray8_C = CreateBitmapWithRandomContent(PixelType::Gray8, width, height);
        Bitmap destGray8_AVX = CreateBitmap(PixelType::Gray8, width, height);
        CopyBitmap(destGray8_AVX, destGray8_C);

        auto start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
            FillFromBitonalFromZeroes_Gray8_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_C.data.get(), destGray8_C.stride, 0x42);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destGray8_AVX.stride * destGray8_AVX.height;
        cout << "Gray8 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Gray8_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_AVX.data.get(), destGray8_AVX.stride, 0x42);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Gray8_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_AVX.data.get(), destGray8_AVX.stride, 0x42);
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
            FillFromBitonalFromZeroes_Gray16_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destGray16_C.data.get(), destGray16_C.stride, 0x4245);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destGray16_AVX.stride * destGray16_AVX.height;
        cout << "Gray16 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Gray16_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destGray16_AVX.data.get(), destGray16_AVX.stride, 0x4245);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Gray16_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destGray16_AVX.data.get(), destGray16_AVX.stride, 0x4245);
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
            FillFromBitonalFromZeroes_Bgr24_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destBgr24_C.data.get(), destBgr24_C.stride, 0x42, 0x44, 0x84);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destBgr24_AVX.stride * destBgr24_AVX.height;
        cout << "Bgr24 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Bgr24_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destBgr24_AVX.data.get(), destBgr24_AVX.stride, 0x42, 0x44, 0x84);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Bgr24_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destBgr24_AVX.data.get(), destBgr24_AVX.stride, 0x42, 0x44, 0x84);
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
            FillFromBitonalFromZeroes_Bgr48_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destBgr48_C.data.get(), destBgr48_C.stride, 0x4252, 0x4454, 0x8494);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destBgr48_AVX.stride * destBgr48_AVX.height;
        cout << "Bgr48 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Bgr48_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destBgr48_AVX.data.get(), destBgr48_AVX.stride, 0x4252, 0x4454, 0x8494);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Bgr48_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destBgr48_AVX.data.get(), destBgr48_AVX.stride, 0x4252, 0x4454, 0x8494);
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
            FillFromBitonalFromZeroes_Float32_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)destFloat32_C.data.get(), destFloat32_C.stride, 1.2345f);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        size_t dataSize = destFloat32_AVX.stride * destFloat32_AVX.height;
        cout << "Float32 (C)" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < REPEAT; ++i)
        {
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Float32_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)destFloat32_AVX.data.get(), destFloat32_AVX.stride, 1.2345f);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Float32_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)destFloat32_AVX.data.get(), destFloat32_AVX.stride, 1.2345f);
#endif

        }

        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        cout << "Float32 (" << SimdName << ")" << " -> " << elapsed_seconds.count() << "s, " << (REPEAT * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

        bool b = Compare(destFloat32_C, destFloat32_AVX);
        cout << "Float32: " << (b == true ? "ok" : "error") << endl;
    }
}

static void TestFromBitonalOnes()
{
    const uint32_t REPEAT = 100;
    const uint32_t width = 2048;
    const uint32_t height = 2048;

#if BITONALFILL_HASAVX
    static const char* SimdName = "AVX";
#elif BITONALFILL_HASNEON
    static const char* SimdName = "NEON";
#endif

    cout << "FillFromOnes:" << endl;
    cout << "=============" << endl;

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
            FillFromBitonalFromOnes_Gray8_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destGray8_AVX.data.get(), destGray8_AVX.stride, 0x42);
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
            FillFromBitonalFromOnes_Gray16_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destGray16_AVX.data.get(), destGray16_AVX.stride, 0x4245);
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
            FillFromBitonalFromOnes_Bgr24_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)destBgr24_AVX.data.get(), destBgr24_AVX.stride, 0x42, 0x44, 0x84);
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
            FillFromBitonalFromOnes_Bgr48_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)destBgr48_AVX.data.get(), destBgr48_AVX.stride, 0x4252, 0x4454, 0x8494);
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
            FillFromBitonalFromOnes_Float32_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)destFloat32_AVX.data.get(), destFloat32_AVX.stride, 1.2345f);
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

static bool TestCaseFromOnes(PixelType pixeltype, uint32_t width, uint32_t height)
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
            FillFromBitonalFromOnes_Gray8_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Gray8_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48);
#endif
            break;
        case PixelType::Gray16:
            FillFromBitonalFromOnes_Gray16_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_C.data.get(), dest_C.stride, 0x4849);
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Gray16_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4849);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Gray16_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4849);
#endif
            break;
        case PixelType::Bgr24:
            FillFromBitonalFromOnes_Bgr24_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_C.data.get(), dest_C.stride, 0x48, 0x49, 0x4a);
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Bgr24_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48, 0x49, 0x4a);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Bgr24_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48, 0x49, 0x4a);
#endif
            break;
        case PixelType::Bgr48:
            FillFromBitonalFromOnes_Bgr48_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_C.data.get(), dest_C.stride, 0x4858, 0x4959, 0x4a5a);
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Bgr48_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4858, 0x4959, 0x4a5a);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Bgr48_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4858, 0x4959, 0x4a5a);
#endif
            break;
        case PixelType::Float32:
            FillFromBitonalFromOnes_Float32_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_C.data.get(), dest_C.stride, 42.4f);
#if BITONALFILL_HASAVX
            FillFromBitonalFromOnes_Float32_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_Simd.data.get(), dest_Simd.stride, 42.4f);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromOnes_Float32_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_Simd.data.get(), dest_Simd.stride, 42.4f);
#endif
            break;
    }

    return Compare(dest_C, dest_Simd);
}

static void StressTestFromOnes()
{
    cout << "Test for 'from ones'" << endl;
    cout << "====================" << endl;
    bool b = TestCaseFromOnes(PixelType::Gray8, 113, 2007);
    cout << "Test 1 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCaseFromOnes(PixelType::Gray16, 113, 2007);
    cout << "Test 2 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCaseFromOnes(PixelType::Bgr24, 113, 2007);
    cout << "Test 3 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCaseFromOnes(PixelType::Bgr48, 113, 2007);
    cout << "Test 4 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCaseFromOnes(PixelType::Float32, 113, 2007);
    cout << "Test 5 -> " << (b == true ? "ok" : "error") << endl;

    b = true;
    for (uint32_t w = 183; w < 200; ++w)
    {
        b &= TestCaseFromOnes(PixelType::Gray8, w, 17);
        b &= TestCaseFromOnes(PixelType::Gray16, w, 17);
        b &= TestCaseFromOnes(PixelType::Bgr24, w, 17);
        b &= TestCaseFromOnes(PixelType::Bgr48, w, 17);
        b &= TestCaseFromOnes(PixelType::Float32, w, 17);
    }

    cout << "Test 6 -> " << (b == true ? "ok" : "error") << endl;
}

static bool TestCaseFromZeroes(PixelType pixeltype, uint32_t width, uint32_t height)
{
    Bitmap bitonal = CreateBitmapWithRandomContent(PixelType::Bitonal, width, height);
    Bitmap dest_C = CreateBitmapWithRandomContent(pixeltype, width, height);
    Bitmap dest_Simd = CreateBitmap(pixeltype, width, height);
    CopyBitmap(dest_Simd, dest_C);
    switch (pixeltype)
    {
        case PixelType::Gray8:
            FillFromBitonalFromZeroes_Gray8_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_C.data.get(), dest_C.stride, 0x48);
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Gray8_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Gray8_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48);
#endif
            break;
        case PixelType::Gray16:
            FillFromBitonalFromZeroes_Gray16_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_C.data.get(), dest_C.stride, 0x4849);
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Gray16_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4849);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Gray16_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4849);
#endif
            break;
        case PixelType::Bgr24:
            FillFromBitonalFromZeroes_Bgr24_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_C.data.get(), dest_C.stride, 0x48, 0x49, 0x4a);
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Bgr24_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48, 0x49, 0x4a);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Bgr24_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint8_t*)dest_Simd.data.get(), dest_Simd.stride, 0x48, 0x49, 0x4a);
#endif
            break;
        case PixelType::Bgr48:
            FillFromBitonalFromZeroes_Bgr48_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_C.data.get(), dest_C.stride, 0x4858, 0x4959, 0x4a5a);
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Bgr48_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4858, 0x4959, 0x4a5a);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Bgr48_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (uint16_t*)dest_Simd.data.get(), dest_Simd.stride, 0x4858, 0x4959, 0x4a5a);
#endif
            break;
        case PixelType::Float32:
            FillFromBitonalFromZeroes_Float32_C(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_C.data.get(), dest_C.stride, 42.4f);
#if BITONALFILL_HASAVX
            FillFromBitonalFromZeroes_Float32_AVX(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_Simd.data.get(), dest_Simd.stride, 42.4f);
#elif BITONALFILL_HASNEON
            FillFromBitonalFromZeroes_Float32_NEON(width, height, (const uint8_t*)bitonal.data.get(), bitonal.stride, (float*)dest_Simd.data.get(), dest_Simd.stride, 42.4f);
#endif
            break;
    }

    return Compare(dest_C, dest_Simd);
}

static void StressTestFromZeroes()
{
    cout << "Test for 'from zeroes'" << endl;
    cout << "======================" << endl;
    bool b = TestCaseFromZeroes(PixelType::Gray8, 113, 2007);
    cout << "Test 1 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCaseFromZeroes(PixelType::Gray16, 113, 2007);
    cout << "Test 2 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCaseFromZeroes(PixelType::Bgr24, 113, 2007);
    cout << "Test 3 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCaseFromZeroes(PixelType::Bgr48, 113, 2007);
    cout << "Test 4 -> " << (b == true ? "ok" : "error") << endl;
    b = TestCaseFromZeroes(PixelType::Float32, 113, 2007);
    cout << "Test 5 -> " << (b == true ? "ok" : "error") << endl;

    b = true;
    for (uint32_t w = 183; w < 200; ++w)
    {
        b &= TestCaseFromZeroes(PixelType::Gray8, w, 17);
        b &= TestCaseFromZeroes(PixelType::Gray16, w, 17);
        b &= TestCaseFromZeroes(PixelType::Bgr24, w, 17);
        b &= TestCaseFromZeroes(PixelType::Bgr48, w, 17);
        b &= TestCaseFromZeroes(PixelType::Float32, w, 17);
    }

    cout << "Test 6 -> " << (b == true ? "ok" : "error") << endl;
}

static void Benchmark_CopyWithBitonalMask_Roi_Gray8()
{
    const uint32_t kWidth = 2048;
    const uint32_t kHeight = 2048;
    const uint32_t kIterations = 100;

    cout << "Benchmark for CopyWithBitonalMask_Roi_Gray8: " << kWidth << "x" << kHeight << ", " << kIterations << " repeats" << endl;

    Bitmap bitonal = CreateBitmapWithRandomContent(PixelType::Bitonal, kWidth, kHeight);
    Bitmap source_gray8 = CreateBitmapWithRandomContent(PixelType::Gray8, kWidth, kHeight);
    Bitmap destination_gray8_C = CreateBitmap(PixelType::Gray8, kWidth - 2, kHeight);
    Bitmap destination_gray8_AVX = CreateBitmap(PixelType::Gray8, kWidth - 2, kHeight);

    memset(destination_gray8_C.data.get(), 0x42, static_cast<size_t>(destination_gray8_C.stride) * destination_gray8_C.height);
    memset(destination_gray8_AVX.data.get(), 0x42, static_cast<size_t>(destination_gray8_AVX.stride) * destination_gray8_AVX.height);

    auto start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < kIterations; ++i)
    {
        CopyWithBitonalMask_Roi_Gray8_C(
            kWidth,
            kHeight,
            static_cast<const uint8_t*>(bitonal.data.get()),
            bitonal.stride,
            static_cast<const uint8_t*>(source_gray8.data.get()),
            source_gray8.stride,
            1,
            0,
            destination_gray8_C.width,
            destination_gray8_C.height,
            static_cast<uint8_t*>(destination_gray8_C.data.get()),
            destination_gray8_C.stride);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    size_t dataSize = static_cast<size_t>(destination_gray8_C.stride) * destination_gray8_C.height;
    cout << "  Gray8 (C)" << " -> " << elapsed_seconds.count() << "s, " << (kIterations * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

#if BITONALFILL_HASAVX || BITONALFILL_HASNEON
#if BITONALFILL_HASAVX
    static const char* SimdName = "AVX";
#elif BITONALFILL_HASNEON
    static const char* SimdName = "NEON";
#endif
    start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < kIterations; ++i)
    {
#if BITONALFILL_HASAVX
        CopyWithBitonalMask_Roi_Gray8_AVX(
            kWidth,
            kHeight,
            (const uint8_t*)bitonal.data.get(),
            bitonal.stride,
            (const uint8_t*)source_gray8.data.get(),
            source_gray8.stride,
            1,
            0,
            destination_gray8_AVX.width,
            destination_gray8_AVX.height,
            (uint8_t*)destination_gray8_AVX.data.get(),
            destination_gray8_AVX.stride);
#endif
#if BITONALFILL_HASNEON
        CopyWithBitonalMask_Roi_Gray8_NEON(
            kWidth,
            kHeight,
            static_cast<const uint8_t*>(bitonal.data.get()),
            bitonal.stride,
            static_cast<const uint8_t*>(source_gray8.data.get()),
            source_gray8.stride,
            1,
            0,
            destination_gray8_AVX.width,
            destination_gray8_AVX.height,
            static_cast<uint8_t*>(destination_gray8_AVX.data.get()),
            destination_gray8_AVX.stride);
#endif
    }

    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end - start;
    dataSize = static_cast<size_t>(destination_gray8_C.stride) * destination_gray8_C.height;
    cout << "  Gray8 (" << SimdName << ")" << " -> " << elapsed_seconds.count() << "s, " << (kIterations * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

    if (Compare(destination_gray8_C, destination_gray8_AVX))
    {
        cout << "  Results compare ok" << endl;
    }
    else
    {
        cout << "  ERROR - results differ" << endl;
    }
#endif
}

static void Benchmark_CopyWithBitonalMask_Roi_Gray16()
{
    const uint32_t kWidth = 2048;
    const uint32_t kHeight = 2048;
    const uint32_t kIterations = 100;

    cout << "Benchmark for CopyWithBitonalMask_Roi_Gray16: " << kWidth << "x" << kHeight << ", " << kIterations << " repeats" << endl;

    Bitmap bitonal = CreateBitmapWithRandomContent(PixelType::Bitonal, kWidth, kHeight);
    Bitmap source_gray16 = CreateBitmapWithRandomContent(PixelType::Gray16, kWidth, kHeight);
    Bitmap destination_gray16_C = CreateBitmap(PixelType::Gray16, kWidth - 2, kHeight);
    Bitmap destination_gray16_AVX = CreateBitmap(PixelType::Gray16, kWidth - 2, kHeight);

    memset(destination_gray16_C.data.get(), 0x42, static_cast<size_t>(destination_gray16_C.stride) * destination_gray16_C.height);
    memset(destination_gray16_AVX.data.get(), 0x42, static_cast<size_t>(destination_gray16_AVX.stride) * destination_gray16_AVX.height);

    auto start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < kIterations; ++i)
    {
        CopyWithBitonalMask_Roi_Gray16_C(
            kWidth,
            kHeight,
            static_cast<const uint8_t*>(bitonal.data.get()),
            bitonal.stride,
            static_cast<const uint16_t*>(source_gray16.data.get()),
            source_gray16.stride,
            1,
            0,
            destination_gray16_C.width,
            destination_gray16_C.height,
            static_cast<uint16_t*>(destination_gray16_C.data.get()),
            destination_gray16_C.stride);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    size_t dataSize = static_cast<size_t>(destination_gray16_C.stride) * destination_gray16_C.height;
    cout << "  Gray16 (C)" << " -> " << elapsed_seconds.count() << "s, " << (kIterations * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

#if BITONALFILL_HASAVX || BITONALFILL_HASNEON
#if BITONALFILL_HASAVX
    static const char* SimdName = "AVX";
#elif BITONALFILL_HASNEON
    static const char* SimdName = "NEON";
#endif
    start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < kIterations; ++i)
    {
#if BITONALFILL_HASAVX
        CopyWithBitonalMask_Roi_Gray16_AVX(
            kWidth,
            kHeight,
            static_cast<const uint8_t*>(bitonal.data.get()),
            bitonal.stride,
            static_cast<const uint16_t*>(source_gray16.data.get()),
            source_gray16.stride,
            1,
            0,
            destination_gray16_AVX.width,
            destination_gray16_AVX.height,
            static_cast<uint16_t*>(destination_gray16_AVX.data.get()),
            destination_gray16_AVX.stride);
#endif
#if BITONALFILL_HASNEON
        CopyWithBitonalMask_Roi_Gray16_NEON(
            kWidth,
            kHeight,
            static_cast<const uint8_t*>(bitonal.data.get()),
            bitonal.stride,
            static_cast<const uint16_t*>(source_gray16.data.get()),
            source_gray16.stride,
            1,
            0,
            destination_gray16_AVX.width,
            destination_gray16_AVX.height,
            static_cast<uint16_t*>(destination_gray16_AVX.data.get()),
            destination_gray16_AVX.stride);
#endif
    }

    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end - start;
    dataSize = static_cast<size_t>(destination_gray16_C.stride) * destination_gray16_C.height;
    cout << "  Gray16 (" << SimdName << ")" << " -> " << elapsed_seconds.count() << "s, " << (kIterations * dataSize / elapsed_seconds.count()) / 1e6 << "MB/s" << endl;

    if (Compare(destination_gray16_C, destination_gray16_AVX))
    {
        cout << "  Results compare ok" << endl;
    }
    else
    {
        cout << "  ERROR - results differ" << endl;
    }
#endif
}

int main()
{
    Benchmark_CopyWithBitonalMask_Roi_Gray8();
    Benchmark_CopyWithBitonalMask_Roi_Gray16();

    cout << endl << endl;

    TestFromBitonalZeroes();
    cout << endl;
    TestFromBitonalOnes();
    cout << endl << endl;
    StressTestFromOnes();
    cout << endl;
    StressTestFromZeroes();

    return 0;
}
