#pragma once

#include <cstdint>
#include <algorithm>

template <typename T>
void FillRemainderLineFromBitonalFromOnes(std::uint32_t count, const std::uint8_t* source, T* destination, T value)
{
    if (count > 0)
    {
        std::uint8_t v = *source;
        std::uint8_t mask = 0x80;
        for (std::uint32_t i = 0; i < std::min(8u, count); ++i)
        {
            if (v & mask)
            {
                destination[i] = value;
            }

            mask >>= 1;
        }

        if (count > 8)
        {
            destination += 8;
            v = *(1 + source);
            mask = 0x80;

            for (std::uint32_t i = 0; i < std::min(8u, count - 8); ++i)
            {
                if (v & mask)
                {
                    destination[i] = value;
                }

                mask >>= 1;
            }
        }
    }
}
