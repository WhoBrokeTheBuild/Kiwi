#ifndef KIWI_DMG_CPU_TYPES_CPU_HPP
#define KIWI_DMG_CPU_TYPES_CPU_HPP

#include <cstdint>

namespace kiwi {
namespace DMG_CPU {
    
// Interrupt Request/Enable Flags for IF and IE
union InterruptFlags
{
    struct {

        // 0x40: V-Blank Interrupt
        bool Int40 : 1;

        // 0x48: LCDC STAT Interrupt
        bool Int48 : 1;

        // 0x50: Timer Interrupt
        bool Int50 : 1;

        // 0x58: Serial Interrupt
        bool Int58 : 1;

        // 0x60: Joypad Interrupt
        bool Int60 : 1;

        uint8_t : 3;
    };

    uint8_t raw;

}; // union InterruptFlags

static_assert(
    sizeof(InterruptFlags) == sizeof(InterruptFlags::raw),
    "sizeof(InterruptFlags) != sizeof(InterruptFlags::raw)"
);

union TimerControl
{
    struct {
        uint8_t Clock : 2;
        bool Enable : 1;
        uint8_t : 5;
    };

    uint8_t raw;

}; // union TimerControl

static_assert(
    sizeof(TimerControl) == sizeof(TimerControl::raw),
    "sizeof(TimerControl) != sizeof(TimerControl::raw)"
);

} // namespace DMG_CPU
} // namespace kiwi

#endif // KIWI_DMG_CPU_TYPES_CPU_HPP