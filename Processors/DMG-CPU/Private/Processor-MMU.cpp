#include <Kiwi/DMG-CPU/Processor.hpp>

namespace kiwi {
namespace DMG_CPU {

uint8_t Processor::readByte(uint16_t address)
{
    switch (address & 0xF000) 
    {
    case 0x0000:
        // $0000-$00FF Bootstrap/Jump Vectors
        if (BootstrapEnabled && address <= 0x00FF) {
            return BootstrapROM[address];
        }
        [[fallthrough]];
    case 0x1000:
    case 0x2000:
    case 0x3000:
        // $0000-$3FFF Cartridge ROM Bank 0
        return CartridgeROM[0][address];

    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        // $4000-$7FFF Cartridge ROM Switchable
        return CartridgeROM[CartridgeROMBank][address - 0x4000];

    case 0x8000:
    case 0x9000:
        // $8000-$9FFF Video RAM
        return VideoRAM[0][address - 0x8000];

    case 0xA000:
    case 0xB000:
        // $A000-$BFFF Cartridge Static RAM
        return StaticRAM[StaticRAMBank][address - 0xA000];

    case 0xC000:
        // $C000-$CFFF Work RAM Bank 0
        return WorkRAM[0][address - 0xC000];

    case 0xD000:
        // $C000-$CFFF Work RAM Switchable
        return WorkRAM[WRAMBank][address - 0xD000];

    case 0xE000:
        // $E000-$EFFF Echo RAM (Work RAM Bank 0)
        return WorkRAM[0][address - 0xE000];

    case 0xF000:
        // $F000-$FDFF Echo RAM (Work RAM Switchable)
        if (address <= 0xFDFF) {
            // ECHO RAM
            return WorkRAM[WRAMBank][address - 0xD000];
        }

        // $FE00-$FE9F Object Attribute Memory
        if (address <= 0xFE9F) {
            return OAM[address - 0xFE00];
        }

        // $FEA0-$FEFF Unusable
        // https://gbdev.io/pandocs/Memory_Map.html#fea0-feff-range
        if (address <= 0xFEFF) {
            return 0xFF;
        }

        // $FF00-$FE7F Hardware I/O Registers
        if (address <= 0xFF7F) {
            // $FF40-$FF3F Wave RAM
            // if (address >= 0xFF30 && address <= 0xFF3F) {
            //     return WaveRAM[address - 0xFF30];
            // }

            switch (address) {
            // $FF00 Joypad
            case 0xFF00:
                return JOYP.raw;

            // $FF01 Serial Transfer Data
            case 0xFF01:

            // $FF02 Serial Transfer Control
            case 0xFF02:

            // $FF04 Divider Register
            case 0xFF04:
                return DIV;

            // $FF05 Timer Accumulator
            case 0xFF05:
                return TIMA;

            // $FF06 Timer Modulo
            case 0xFF06:
                return TMA;

            // $FF07 Timer Control
            case 0xFF07:
                return TAC.raw;

            // $FF0F Requested Interrupts
            case 0xFF0F:
                return IF.raw;

            default:
                return 0xFF;
            }
        }

        // $FF80-$FFFE High RAM / Zero Page
        if (address <= 0xFFFE) {
            return HighRAM[address - 0xFF80];
        }

        // $FFFE Enabled Interrupts
        return IE.raw;
    };

    return 0;
}

void Processor::writeByte(uint16_t address, uint8_t data)
{
    // switch (address) 
    // {

    // };
}

} // namespace DMG_CPU
} // namespace kiwi