#include <cstdlib>
#include "CopyWithBitonalMask.h"
#include "ColorPixelStructs.h"

using namespace std;

template<typename T>
struct CopyWithBitonalMaskData
{
    uint32_t width;
    uint32_t height;
    const uint8_t* bitonal_data;
    uint32_t bitonal_stride;
    const T* source_data;
    uint32_t source_stride;
    T* destination_data;
    uint32_t destination_stride;
};

template <typename T>
void CopyWithBitonalMask_C(const CopyWithBitonalMaskData<T>& info)
{
    const uint32_t width_over_eight = info.width / 8;
    const uint32_t width_remainder = info.width % 8;

    for (uint32_t y = 0; y < info.height; ++y)
    {
        const T* source = reinterpret_cast<const T*>(reinterpret_cast<const uint8_t*>(info.source_data) + static_cast<size_t>(y) * info.source_stride);
        T* destination = reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(info.destination_data) + static_cast<size_t>(y) * info.destination_stride);
        const uint8_t* bitonal = info.bitonal_data + static_cast<size_t>(y) * info.bitonal_stride;

        for (uint32_t x_over_eight = 0; x_over_eight < width_over_eight; ++x_over_eight)
        {
            const uint8_t v = *bitonal;
            uint8_t mask = 0x80;
            for (uint8_t i = 0; i < 8; ++i)
            {
                if (v & mask)
                {
                    destination[i] = source[i];
                }

                mask >>= 1;
            }

            ++bitonal;
            source += 8;
            destination += 8;
        }

        if (width_remainder > 0)
        {
            uint8_t v = *bitonal;
            uint8_t mask = 0x80;
            for (uint32_t i = 0; i < width_remainder; ++i)
            {
                if (v & mask)
                {
                    destination[i] = source[i];
                }

                mask >>= 1;
            }
        }
    }
}

void CopyWithBitonalMask_Gray8_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint8_t* source,
    std::uint32_t source_stride,
    std::uint8_t* destination,
    std::uint32_t destination_stride)
{
    const CopyWithBitonalMaskData<uint8_t> info = { width, height, source_bitonal, source_bitonal_stride, source, source_stride, destination, destination_stride };
    CopyWithBitonalMask_C<uint8_t>(info);
}

void CopyWithBitonalMask_Gray16_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint16_t* destination,
    std::uint32_t destination_stride)
{
    const CopyWithBitonalMaskData<uint16_t> info = { width, height, source_bitonal, source_bitonal_stride, source, source_stride, destination, destination_stride };
    CopyWithBitonalMask_C<uint16_t>(info);
}

template<typename T>
struct CopyRoiWithBitonalMaskData
{
    uint32_t width;
    uint32_t height;
    const uint8_t* bitonal_data;
    uint32_t bitonal_stride;
    const T* source_data;
    uint32_t source_stride;
    std::uint32_t roi_x;
    std::uint32_t roi_y;
    std::uint32_t roi_width;
    std::uint32_t roi_height;
    T* destination_data;
    uint32_t destination_stride;
};

template <typename T>
void CopyWithBitonalMask_Roi_Gray8_C(const CopyRoiWithBitonalMaskData<T>& info)
{
    const uint32_t start_shift = info.roi_x % 8;
    const uint32_t width_over_eight = (info.roi_width - start_shift) / 8;
    const uint32_t width_remainder = (info.roi_width - start_shift) % 8;

    const uint8_t* source_pointer = reinterpret_cast<const uint8_t*>(info.source_data) + static_cast<size_t>(info.roi_y) * info.source_stride + static_cast<size_t>(info.roi_x) * sizeof(T);
    const uint8_t* bitonal_pointer = reinterpret_cast<const uint8_t*>(info.bitonal_data) + static_cast<size_t>(info.roi_y) * info.bitonal_stride + info.roi_x / 8;

    for (uint32_t y = 0; y < info.height; ++y)
    {
        const T* source = reinterpret_cast<const T*>(source_pointer + static_cast<size_t>(y) * info.source_stride);
        T* destination = reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(info.destination_data) + static_cast<size_t>(y) * info.destination_stride);
        const uint8_t* bitonal = bitonal_pointer + static_cast<size_t>(y) * info.bitonal_stride;

        if (start_shift > 0)
        {
            // we do the first few pixels manually, until we are aligned to 8 pixels (or: the whole mask byte is used)
            for (uint32_t i = start_shift; i < 8; ++i)
            {
                const uint8_t v = *bitonal;
                if (v & (0x80 >> i))
                {
                    *destination = *source;
                }

                ++destination;
                ++source;
            }

            ++bitonal;
        }

        for (uint32_t x_over_eight = 0; x_over_eight < width_over_eight; ++x_over_eight)
        {
            const uint8_t v = *bitonal;
            uint8_t mask = 0x80;
            for (uint8_t i = 0; i < 8; ++i)
            {
                if (v & mask)
                {
                    destination[i] = source[i];
                }

                mask >>= 1;
            }

            ++bitonal;
            source += 8;
            destination += 8;
        }

        if (width_remainder > 0)
        {
            uint8_t v = *bitonal;
            uint8_t mask = 0x80;
            for (uint32_t i = 0; i < width_remainder; ++i)
            {
                if (v & mask)
                {
                    destination[i] = source[i];
                }

                mask >>= 1;
            }
        }
    }
}

void CopyWithBitonalMask_Roi_Gray8_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint8_t* source,
    std::uint32_t source_stride,
    std::uint32_t roi_x,
    std::uint32_t roi_y,
    std::uint32_t roi_width,
    std::uint32_t roi_height,
    std::uint8_t* destination,
    std::uint32_t destination_stride)
{
    const CopyRoiWithBitonalMaskData<uint8_t> info = { width, height, source_bitonal, source_bitonal_stride, source, source_stride, roi_x, roi_y, roi_width, roi_height, destination, destination_stride };
    CopyWithBitonalMask_Roi_Gray8_C<uint8_t>(info);
}
