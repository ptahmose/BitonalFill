#include "HiLoBytePackUnpack.h"
#include "config.h"

#if BITONALFILL_HASNEON
#include <arm_neon.h>

using namespace std;

void LoHiByteUnpack_NEON(std::uint32_t width, std::uint32_t height, std::uint32_t stride, const void* source, void* dest)
{
    uint8_t* pDst = static_cast<uint8_t*>(dest);
    const uint32_t widthOver8 = width / 8;
    const uint32_t widthModulo8 = width % 8;
    const size_t halfLength = (static_cast<size_t>(width) * 2 * height) / 2;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* pSrc = reinterpret_cast<const uint16_t*>(static_cast<const uint8_t*>(source) + y * static_cast<size_t>(stride));

        for (uint32_t i = 0; i < widthOver8; ++i)
        {
            const uint8x8x2_t data = vld2_u8(pSrc);
            vst1_u8(pDst, data.val[0]);
            vst1_u8(pDst + halfLength, data.val[1]);

            pSrc += 8;  // we do 8 words = 16 bytes per loop
            pDst += 8;
        }

        for (uint32_t i = 0; i < widthModulo8; ++i)
        {
            const uint16_t v = *pSrc++;
            *pDst = static_cast<uint8_t>(v);
            *(pDst + halfLength) = static_cast<uint8_t>(v >> 8);
            pDst++;
        }
    }
}

#endif