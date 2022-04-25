#include <immintrin.h>
#include "FillFromBitonal.h"

using namespace std;

void FillFromBitonalFromOnes_Gray8_AVX2(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* sourceBitonal,
    std::uint32_t sourceBitonalStride,
    std::uint8_t* destination,
    std::uint32_t destinationStride,
    std::uint8_t valueForOnes)
{
    uint8_t v = 0x12;

    __m128i value = _mm_set1_epi8(static_cast<char>(valueForOnes));
    __m128i zero = _mm_setzero_si128();
    __m128i mask = _mm_set_epi8(0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01);
    __m128i m1 = _mm_set1_epi8(static_cast<char>(v));
    __m128i m2 = _mm_andnot_si128(m1, mask);
    __m128i m3 = _mm_cmpeq_epi8(m2, zero);
    _mm_maskmoveu_si128(value, m3, reinterpret_cast<char*>(destination));
}