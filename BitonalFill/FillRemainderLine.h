#pragma once

#include <cstdint>
#include <algorithm>

/// This function is used for processing at most 16 bits (=2 bytes), and this operation is done with bytes-wise access.
/// It is intended for processing a remainder (in a line)
///
/// \tparam T   Generic type parameter.
/// \param          count       Number of pixels to process (must be less or equal to 16).
/// \param          source      Bitonal bitmap.
/// \param [in,out] destination The destination bitmap.
/// \param          value       The value for "ones".
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

/// This function is used for processing at most 16 bits (=2 bytes), and this operation is done with bytes-wise access.
/// It is intended for processing a remainder (in a line)
///
/// \tparam T   Generic type parameter.
/// \param          count       Number of pixels to process (must be less or equal to 16).
/// \param          source      Bitonal bitmap.
/// \param [in,out] destination The destination bitmap.
/// \param          value       The value for "zeroes".
template <typename T>
void FillRemainderLineFromBitonalFromZeroes(std::uint32_t count, const std::uint8_t* source, T* destination, T value)
{
    if (count > 0)
    {
        std::uint8_t v = *source;
        std::uint8_t mask = 0x80;
        for (std::uint32_t i = 0; i < std::min(8u, count); ++i)
        {
            if (!(v & mask))
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
                if (!(v & mask))
                {
                    destination[i] = value;
                }

                mask >>= 1;
            }
        }
    }
}
