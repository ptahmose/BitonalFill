#include "CopyWithBitonalMask.h"
#include "CopyWithBitonalMaskUtilities.h"
#include "ColorPixelStructs.h"
#include <cstdlib>
#include <algorithm>


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
/*
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
}*/

//void CopyWithBitonalMask_Gray8_C(
//    std::uint32_t width,
//    std::uint32_t height,
//    const std::uint8_t* source_bitonal,
//    std::uint32_t source_bitonal_stride,
//    const std::uint8_t* source,
//    std::uint32_t source_stride,
//    std::uint8_t* destination,
//    std::uint32_t destination_stride)
//{
//    const CopyWithBitonalMaskData<uint8_t> info = { width, height, source_bitonal, source_bitonal_stride, source, source_stride, destination, destination_stride };
//    CopyWithBitonalMask_C<uint8_t>(info);
//}
//
//void CopyWithBitonalMask_Gray16_C(
//    std::uint32_t width,
//    std::uint32_t height,
//    const std::uint8_t* source_bitonal,
//    std::uint32_t source_bitonal_stride,
//    const std::uint16_t* source,
//    std::uint32_t source_stride,
//    std::uint16_t* destination,
//    std::uint32_t destination_stride)
//{
//    const CopyWithBitonalMaskData<uint16_t> info = { width, height, source_bitonal, source_bitonal_stride, source, source_stride, destination, destination_stride };
//    CopyWithBitonalMask_C<uint16_t>(info);
//}

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
void CopyWithBitonalMask_Roi_C(const CopyRoiWithBitonalMaskData<T>& info)
{

    // This is the number of bits we need to do before we are byte aligned - we round up the roi_x to the next byte boundary, and then subtract roi_x.
    // Important - this must not be larger than the roi_width.
    const uint32_t bits_to_byte_alignment = std::min(((info.roi_x + 7) / 8) * 8 - info.roi_x, info.roi_width);

    // now, we only have to consider the remaining bits, i.e. the number of bits calculated above needs to be subtracted from the roi_width.
    const uint32_t remaining_bits_count = info.roi_width - bits_to_byte_alignment;   // note: this cannot be negative
    const uint32_t width_over_eight = remaining_bits_count / 8;
    const uint32_t width_remainder = remaining_bits_count % 8;

    const uint8_t* source_pointer = reinterpret_cast<const uint8_t*>(info.source_data) + static_cast<size_t>(info.roi_y) * info.source_stride + static_cast<size_t>(info.roi_x) * sizeof(T);
    const uint8_t* bitonal_pointer = reinterpret_cast<const uint8_t*>(info.bitonal_data) + static_cast<size_t>(info.roi_y) * info.bitonal_stride + info.roi_x / 8;

    for (uint32_t y = 0; y < info.height; ++y)
    {
        const T* source = reinterpret_cast<const T*>(source_pointer + static_cast<size_t>(y) * info.source_stride);
        T* destination = reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(info.destination_data) + static_cast<size_t>(y) * info.destination_stride);
        const uint8_t* bitonal = bitonal_pointer + static_cast<size_t>(y) * info.bitonal_stride;

        // First, we do the bits until we are byte aligned.
        if (bits_to_byte_alignment > 0)
        {
            const uint8_t bitonal_byte = *bitonal;
            for (uint32_t i = 8 - bits_to_byte_alignment; i < 8; ++i)
            {
                const uint8_t bit = 0x80 >> i;
                if (bitonal_byte & bit)
                {
                    *destination = *source;
                }

                ++source;
                ++destination;
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

int CopyWithBitonalMask_Roi_Gray8_C(
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
    int error_code = CheckCopyWithBitonalMask_Roi_Parameters(width, height, roi_x, roi_y, roi_width, roi_height);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_BitonalSource_Parameters(width, height, source_bitonal, source_bitonal_stride);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_Bitmap_Parameters(width, height, source, source_stride, 1);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_Bitmap_Parameters(roi_width, roi_height, destination, destination_stride, 1);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    const CopyRoiWithBitonalMaskData<uint8_t> info = { width, height, source_bitonal, source_bitonal_stride, source, source_stride, roi_x, roi_y, roi_width, roi_height, destination, destination_stride };
    CopyWithBitonalMask_Roi_C<uint8_t>(info);
    return ReturnCode_Success;
}

int CopyWithBitonalMask_Roi_Gray16_C(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride,
    const std::uint16_t* source,
    std::uint32_t source_stride,
    std::uint32_t roi_x,
    std::uint32_t roi_y,
    std::uint32_t roi_width,
    std::uint32_t roi_height,
    std::uint16_t* destination,
    std::uint32_t destination_stride)
{
    int error_code = CheckCopyWithBitonalMask_Roi_Parameters(width, height, roi_x, roi_y, roi_width, roi_height);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_BitonalSource_Parameters(width, height, source_bitonal, source_bitonal_stride);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_Bitmap_Parameters(width, height, source, source_stride, 1);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    error_code = CheckCopyWithBitonalMask_Bitmap_Parameters(roi_width, roi_height, destination, destination_stride, 1);
    if (error_code != ReturnCode_Success)
    {
        return error_code;
    }

    const CopyRoiWithBitonalMaskData<uint16_t> info = { width, height, source_bitonal, source_bitonal_stride, source, source_stride, roi_x, roi_y, roi_width, roi_height, destination, destination_stride };
    CopyWithBitonalMask_Roi_C<uint16_t>(info);
    return ReturnCode_Success;
}
