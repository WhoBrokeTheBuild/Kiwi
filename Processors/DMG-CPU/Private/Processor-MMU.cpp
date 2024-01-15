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
        return WorkRAM[WorkRAMBank][address - 0xD000];

    case 0xE000:
        // $E000-$EFFF Echo RAM (Work RAM Bank 0)
        return WorkRAM[0][address - 0xE000];

    case 0xF000:
        // $F000-$FDFF Echo RAM (Work RAM Switchable)
        if (address <= 0xFDFF) {
            // ECHO RAM
            return WorkRAM[WorkRAMBank][address - 0xF000];
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
                break;

            // $FF02 Serial Transfer Control
            case 0xFF02:
                break;

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

            // $FF40 LCD Control
            case 0xFF40:
                return LCDC.raw;

            // $FF41 LCD Status
            case 0xFF41:
                return STAT.raw;
                    
            // $FF42 Scroll Y
            case 0xFF42:
                return SCY;

            // $FF43 Scroll X
            case 0xFF43:
                return SCX;

            // $FF44 Current Line
            case 0xFF44:
                return LY;

            // $FF45 Line Coincidence
            case 0xFF45:
                return LYC;
    
            // $FF47 Background palette
            case 0xFF47:
                return BGP.raw;

            // $FF48 Sprite palette 0
            case 0xFF48:
                return OBP0.raw;

            // $FF49 Sprite palette 1
            case 0xFF49:
                return OBP1.raw;

            // $FF4A Window Y
            case 0xFF4A:
                return WY;

            // $FF4B Window X
            case 0xFF4B:
                return WX;

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
    switch (address & 0xF000) 
    {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        // $0000-$7FFF Cartridge MBC
        break;

    case 0x8000:
    case 0x9000:
        // $8000-$9FFF Video RAM
        VideoRAM[0][address - 0x8000] = data;
        break;

    case 0xA000:
    case 0xB000:
        // $A000-$BFFF Cartridge Static RAM
        StaticRAM[StaticRAMBank][address - 0xA000] = data;
        break;

    case 0xC000:
        // $C000-$CFFF Work RAM Bank 0
        WorkRAM[0][address - 0xC000] = data;
        break;

    case 0xD000:
        // $C000-$CFFF Work RAM Switchable
        WorkRAM[WorkRAMBank][address - 0xD000] = data;
        break;

    case 0xE000:
        // $E000-$EFFF Echo RAM (Work RAM Bank 0)
        WorkRAM[0][address - 0xE000] = data;
        break;

    case 0xF000:
        // $F000-$FDFF Echo RAM (Work RAM Switchable)
        if (address <= 0xFDFF) {
            // ECHO RAM
            WorkRAM[WorkRAMBank][address - 0xF000] = data;
            break;
        }
        
        // $FE00-$FE9F Object Attribute Memory
        if (address <= 0xFE9F) {
            OAM[address - 0xFE00] = data;
            break;
        }
        
        // $FF00-$FE7F Hardware I/O Registers
        if (address <= 0xFF7F) {
            // $FF40-$FF3F Wave RAM
            // if (address >= 0xFF30 && address <= 0xFF3F) {
            //     WaveRAM[address - 0xFF30] = data;
            // }

            switch (address) {
            // $FF00 Joypad
            case 0xFF00:
                // TODO:
                JOYP.raw ^= 0b00110000;
                JOYP.raw |= data & 0b00110000;
                break;

            // $FF01 Serial Transfer Data
            case 0xFF01:
                break;

            // $FF02 Serial Transfer Control
            case 0xFF02:
                break;

            // $FF04 Divider Register
            case 0xFF04:
                DIV = 0;
                break;

            // $FF05 Timer Accumulator
            case 0xFF05:
                TIMA = data;
                break;

            // $FF06 Timer Modulo
            case 0xFF06:
                TMA = data;
                break;

            // $FF07 Timer Control
            case 0xFF07:
                TAC.raw = data;
                break;

            // $FF0F Requested Interrupts
            case 0xFF0F:
                IF.raw = data;
                break;

            // $FF40 LCD Control
            case 0xFF40:
                LCDC.raw = data;
                break;

            // $FF41 LCD Status
            case 0xFF41:
                STAT.raw ^= 0b01111000;
                STAT.raw |= data & 0b01111000;
                break;
                    
            // $FF42 Scroll Y
            case 0xFF42:
                SCY = data;
                break;

            // $FF43 Scroll X
            case 0xFF43:
                SCX = data;
                break;

            // $FF44 Current Line
            case 0xFF44:
                LY = data;
                break;

            // $FF45 Line Coincidence
            case 0xFF45:
                LYC = data;
                break;
    
            // $FF47 Background palette
            case 0xFF47:
                BGP.raw = data;
                break;

            // $FF48 Sprite palette 0
            case 0xFF48:
                OBP0.raw = data;
                break;

            // $FF49 Sprite palette 1
            case 0xFF49:
                OBP1.raw = data;
                break;

            // $FF4A Window Y
            case 0xFF4A:
                WY = data;
                break;

            // $FF4B Window X
            case 0xFF4B:
                WX = data;
                break;
            }
            
            // VERY IMPORTANT
            break;
        }
        
        // $FF80-$FFFE High RAM / Zero Page
        if (address <= 0xFFFE) {
            HighRAM[address - 0xFF80] = data;
            break;
        }

        // $FFFE Enabled Interrupts
        IE.raw = data;
        break;
    }
}

} // namespace DMG_CPU
} // namespace kiwi