#include "HiLoBytePackUnpack.h"

using namespace std;

void LoHiByteUnpack_C(std::uint32_t width, std::uint32_t height, std::uint32_t stride, const void* source, void* dest)
{
    uint8_t* pDst = static_cast<uint8_t*>(dest);

    const size_t halfLength = (static_cast<size_t>(width) * 2 * height) / 2;
    for (uint32_t y = 0; y < height; ++y)
    {
        const uint16_t* pSrc = reinterpret_cast<const uint16_t*>(static_cast<const uint8_t*>(source) + y * static_cast<size_t>(stride));
        for (uint32_t x = 0; x < width; ++x)
        {
            const uint16_t v = *pSrc++;
            *pDst = static_cast<uint8_t>(v);
            *(pDst + halfLength) = static_cast<uint8_t>(v >> 8);
            pDst++;
        }
    }
}

