#include "HiLoBytePackUnpack.h"
#include "config.h"

#if BITONALFILL_HASAVX
#include <immintrin.h>

using namespace std;

void LoHiByteUnpack_AVX(std::uint32_t width, std::uint32_t height, std::uint32_t stride, const void* source, void* dest)
{
    static const __m128i shuffleConst128 = _mm_set_epi8(
        0xf, 0xd, 0xb, 0x9,
        0x7, 0x5, 0x3, 0x1,
        0xe, 0xc, 0xa, 0x8,
        0x6, 0x4, 0x2, 0x0);

    uint8_t* pDst = static_cast<uint8_t*>(dest);
    const size_t halfLength = (static_cast<size_t>(width) * 2 * height) / 2;
    const uint32_t widthOver16 = width / 16;
    const uint32_t widthModulo16 = width % 16;
    const __m256i shuffleConst = _mm256_broadcastsi128_si256(shuffleConst128);
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* pSrc = reinterpret_cast<const uint16_t*>(static_cast<const uint8_t*>(source) + y * static_cast<size_t>(stride));

        for (uint32_t i = 0; i < widthOver16; ++i)
        {
            const __m256i d = _mm256_lddqu_si256((const __m256i*)pSrc);
            const __m256i shuffled = _mm256_permute4x64_epi64(_mm256_shuffle_epi8(d, shuffleConst), 0xd8);
            _mm256_storeu2_m128i(reinterpret_cast<__m128i*>(pDst + halfLength), reinterpret_cast<__m128i*>(pDst), shuffled);

            pSrc += 16;  // we do 16 words = 32 bytes per loop
            pDst += 16;
        }

        for (uint32_t i = 0; i < widthModulo16; ++i)
        {
            const uint16_t v = *pSrc++;
            *pDst = static_cast<uint8_t>(v);
            *(pDst + halfLength) = static_cast<uint8_t>(v >> 8);
            pDst++;
        }
    }
}
#endif
