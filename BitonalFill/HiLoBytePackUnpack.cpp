#include "HiLoBytePackUnpack.h"
#include <stdexcept>
#include <sstream>

using namespace std;

void CheckLoHiByteUnpackArgumentsAndThrow(std::uint32_t width, std::uint32_t stride, const void* source, void* dest)
{
    if (source == nullptr)
    {
        throw invalid_argument("'source' must not be null.");
    }

    if (dest == nullptr)
    {
        throw invalid_argument("'dest' must not be null.");
    }

    if (stride < width * 2)
    {
        stringstream ss;
        ss << "For a width of " << width << " pixels, the stride must be >= " << width * 2 << ".";
        throw invalid_argument(ss.str());
    }
}

void CheckLoHiBytePackArgumentsAndThrow(const void* ptrSrc, size_t sizeSrc, std::uint32_t width, std::uint32_t height, std::uint32_t stride, void* dest)
{
    if (ptrSrc == nullptr)
    {
        throw invalid_argument("'ptrSrc' must not be null.");
    }

    if (dest == nullptr)
    {
        throw invalid_argument("'dest' must not be null.");
    }

    if (sizeSrc < static_cast<size_t>(width) * height * 2)
    {
        stringstream ss;
        ss << "For a width of " << width << " pixels and a height of " << height << ", the 'sizeSrc' must be >= " << static_cast<size_t>(width) * height * 2 << ".";
        throw invalid_argument(ss.str());
    }

    if (stride < width * 2)
    {
        stringstream ss;
        ss << "For a width of " << width << " pixels, the stride must be >= " << width * 2 << ".";
        throw invalid_argument(ss.str());
    }
}

void LoHiByteUnpack_C(std::uint32_t width, std::uint32_t height, std::uint32_t stride, const void* source, void* dest)
{
    CheckLoHiByteUnpackArgumentsAndThrow(width, stride, source, dest);

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

void LoHiBytePack_C(const void* ptrSrc, size_t sizeSrc, std::uint32_t width, std::uint32_t height, std::uint32_t stride, void* dest)
{
    CheckLoHiBytePackArgumentsAndThrow(ptrSrc, sizeSrc, width, height, stride, dest);

    const uint8_t* pSrc = static_cast<const uint8_t*>(ptrSrc);

    const size_t halfLength = sizeSrc / 2;

    for (uint32_t y = 0; y < height; ++y)
    {
        uint16_t* pDst = reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(dest) + static_cast<size_t>(y) * stride);
        for (uint32_t x = 0; x < width; ++x)
        {
            const uint16_t v = *pSrc | (static_cast<uint16_t>(*(pSrc + halfLength)) << 8);
            *pDst++ = v;
            ++pSrc;
        }
    }
}
