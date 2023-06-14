#include "CopyWithBitonalMaskUtilities.h"

int CheckCopyWithBitonalMask_Roi_Parameters(
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t roi_x,
    std::uint32_t roi_y,
    std::uint32_t roi_width,
    std::uint32_t roi_height)
{
    if (width == 0 || height == 0)
    {
        return ReturnCode_InvalidArgument;
    }

    if (roi_width == 0 || roi_height == 0)
    {
        return ReturnCode_InvalidRoi;
    }

    if (roi_x + roi_width > width || roi_y + roi_height > height)
    {
        return ReturnCode_InvalidRoi;
    }

    return ReturnCode_Success;
}

int CheckCopyWithBitonalMask_BitonalSource_Parameters(
    std::uint32_t width,
    std::uint32_t height,
    const std::uint8_t* source_bitonal,
    std::uint32_t source_bitonal_stride)
{
    if (width == 0 || height == 0)
    {
        return ReturnCode_InvalidArgument;
    }

    if (source_bitonal == nullptr)
    {
        return ReturnCode_InvalidPointers;
    }

    // check whether the specified stride is large enough to hold a row of bitonal data
    if ((width + 7) / 8 > source_bitonal_stride)
    {
        return ReturnCode_InvalidStride;
    }

    return ReturnCode_Success;
}

int CheckCopyWithBitonalMask_Bitmap_Parameters(
    std::uint32_t width,
    std::uint32_t height,
    const void* bitmap_data,
    std::uint32_t bitmap_stride,
    size_t size_of_pixel)
{
    if (bitmap_data == nullptr)
    {
        return ReturnCode_InvalidPointers;
    }

    if (width == 0 || height == 0)
    {
        return ReturnCode_InvalidArgument;
    }

    // check whether the specified stride is large enough to hold a row of bitmap data
    if (width * size_of_pixel > bitmap_stride)
    {
        return ReturnCode_InvalidStride;
    }

    return ReturnCode_Success;
}
