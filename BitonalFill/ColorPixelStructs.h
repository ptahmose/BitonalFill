#pragma once

#include <cstdint>

#pragma pack(push)
#pragma pack(1)
struct BgrGray8
{
    std::uint8_t b;
    std::uint8_t g;
    std::uint8_t r;
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct BgrGray16
{
    std::uint16_t b;
    std::uint16_t g;
    std::uint16_t r;
};
#pragma pack(pop)