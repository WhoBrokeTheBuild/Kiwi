#ifndef KIWI_DMG_CPU_TYPES_PPU_HPP
#define KIWI_DMG_CPU_TYPES_PPU_HPP

#include <cstdint>

namespace kiwi {
namespace DMG_CPU {

// Color palette
union Palette
{
    struct {
        uint8_t Color0 : 2;
        uint8_t Color1 : 2;
        uint8_t Color2 : 2;
        uint8_t Color3 : 2;
    };

    uint8_t raw;

}; // union Palette

static_assert(
    sizeof(Palette) == sizeof(Palette::raw),
    "sizeof(Palette) != sizeof(Palette::raw)"
);

} // namespace DMG_CPU
} // namespace kiwi

#endif // KIWI_DMG_CPU_TYPES_PPU_HPP