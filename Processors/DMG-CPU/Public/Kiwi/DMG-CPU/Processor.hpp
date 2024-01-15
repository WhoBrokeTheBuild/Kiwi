#ifndef KIWI_DMG_CPU_HPP
#define KIWI_DMG_CPU_HPP

#include <Kiwi/Utility/Macros.hpp>
#include <Kiwi/Utility/String.hpp>

#include <Kiwi/DMG-CPU/Constants.hpp>
#include <Kiwi/DMG-CPU/Types/CPU.hpp>
#include <Kiwi/DMG-CPU/Types/PPU.hpp>

#include <cstdint>

namespace kiwi {
namespace DMG_CPU {

class Processor
{
public:

    ///
    /// General
    ///

    // Output Pixel Data
    uint8_t LCD[SCREEN_BUFFER_SIZE];

    // Output Volume (0.0 - 1.0)
    float Volume;

    bool CPUEnabled;

    bool PPUEnabled;

    bool SRAMEnabled;

    ///
    /// Memory
    ///

    uint8_t BootstrapROM[BOOTSTRAP_ROM_SIZE];

    // True if there is a Bootstrap ROM loaded
    bool BootstrapLoaded;

    // $FF50 Controls whether $0000-$0100 will map to BootstrapROM or CartridgeROM[0]
    bool BootstrapEnabled;
    
    // $8000-$9FFF Video RAM
    uint8_t VideoRAM[VIDEO_RAM_BANK_COUNT][VIDEO_RAM_BANK_SIZE];

    //
    int VideoRAMBank;

    // $FE00-$FE9F Object Attribute Memory
    uint8_t OAM[OAM_SIZE];

    // $0000-$3FFF Cartridge ROM Bank 0
    // $4000-$7FFF Cartridge ROM Switchable
    uint8_t (*CartridgeROM)[CARTRIDGE_ROM_BANK_SIZE] = nullptr;

    // MBC-controlled Cartridge ROM Bank index for $4000-$7FFF
    unsigned CartridgeROMBank;

    // $A000-$BFFF Cartridge Static RAM
    uint8_t StaticRAM[STATIC_RAM_BANK_COUNT][STATIC_RAM_BANK_SIZE];

    // MBC-controlled Cartridge Static RAM Bank index for $A000-$BFFF
    unsigned StaticRAMBank;

    // $C000-$CFFF Work RAM Bank 0 (Echo at $E000-$EFFF)
    // $C000-$CFFF Work RAM Switchable (Echo at $F000-$FDFF)
    uint8_t WorkRAM[WORK_RAM_BANK_COUNT][WORK_RAM_BANK_SIZE];

    // 
    unsigned WorkRAMBank;

    // $FF80-$FFFE High RAM / Zero Page
    uint8_t HighRAM[HIGH_RAM_SIZE];


    ///
    /// Registers
    ///

    union {
        struct {
            union {
                struct {
                    uint8_t : 4;

                    // Zero Flag
                    bool FZ : 1;

                    // Subtract Flag
                    bool FN : 1;

                    // Half-Carry Flag
                    bool FH : 1;

                    // Carry Flag
                    bool FC : 1;
                };
                
                // Raw Flags Register
                uint8_t F;
            };

            // Accumulator Register
            uint8_t A;
        };

        // (A << 8) | F
        uint16_t AF;
    };

    union {
        struct {
            // General Purpose Register
            uint8_t C;

            // General Purpose Register
            uint8_t B;
        };

        // (B << 8) | C
        uint16_t BC;
    };

    union {
        struct {
            // General Purpose Register
            uint8_t E;
            
            // General Purpose Register
            uint8_t D;
        };

        // (D << 8) | E
        uint16_t DE;
    };

    union {
        struct {
            // Low Address Register
            uint8_t L;

            // High Address Register
            uint8_t H;
        };
        
        // (H << 8) | L
        uint16_t HL;
    };

    // Program Counter
    uint16_t PC;

    // Stack Pointer
    uint16_t SP;

    // Interrupt Master Enable
    bool IME;

    // Enable IME after the next instruction
    bool RequestIME;

    ///
    /// Hardware I/O Registers
    ///

    // $FF0F Requested Interrupts
    InterruptFlags IF;

    // $FFFE Enabled Interrupts
    InterruptFlags IE;

    // $FF42 Scroll Y
    uint8_t SCY;

    // $FF43 Scroll X
    uint8_t SCX;

    // $FF44 Current Line
    uint8_t LY;

    // $FF45 Line Coincidence
    uint8_t LYC;
    
    // $FF4A Window Y
    uint8_t WY;

    // $FF4B Window X
    uint8_t WX;

    // $FF47 Background palette
    Palette BGP;

    // $FF48 Sprite palette 0
    Palette OBP0;

    // $FF49 Sprite palette 1
    Palette OBP1;

    union {
        struct {
            uint8_t                 : 4;
            bool SelectAction       : 1;
            bool SelectDirection    : 1;
            uint8_t                 : 2;
        };

        struct {
            bool Right              : 1;
            bool Left               : 1;
            bool Up                 : 1;
            bool Down               : 1;
            uint8_t                 : 4;
        };

        struct {
            bool A                  : 1;
            bool B                  : 1;
            bool Select             : 1;
            bool Start              : 1;
            uint8_t                 : 4;
        };

        uint8_t raw;
        
    } JOYP;

    // $FF04 Divider Register
    uint8_t DIV;

    // $FF05 Timer Accumulator
    uint8_t TIMA;

    // $FF06 Timer Modulo
    uint8_t TMA;

    // $FF07 Timer Control
    TimerControl TAC;

    // $FF40 LCD Control
    union {
        struct {
            bool TileDisplayEnable      : 1;
            bool SpriteDisplayEnable    : 1;
            uint8_t SpriteSize          : 1;
            uint8_t BGTileMapSelect     : 1;
            uint8_t TileDataSelect      : 1;
            bool WindowEnable           : 1;
            uint8_t WindowTileMapSelect : 1;
            bool Enable                 : 1;
        };

        uint8_t raw;

    } LCDC;

    // $FF41 LCD Status
    union {
        KIWI_PACK(
            struct {
                uint8_t Mode : 2;
                bool Coincidence : 1; // True when LY == LYC
                bool IntHBlank : 1;
                bool IntVBlank : 1;
                bool IntSearchSprite : 1;
                bool IntCoincidence : 1;
                uint8_t : 1;
            }
        );

        uint8_t raw;

    } STAT;

    Processor();
    
    virtual ~Processor();

    void loadBootstrapROM(const String& filename);

    void loadCartridgeROM(const String& filename);

    void reset();

    void tick();

private:

    unsigned _lineTicks = 0;

    void checkInterrupts();

    void tickCPU();

    void tickPPU();

    void tickAPU();

    uint8_t readByte(uint16_t address);

    void writeByte(uint16_t address, uint8_t data);
    
    KIWI_FORCE_INLINE
    uint16_t readWord(uint16_t address) {
        uint8_t lower = readByte(address);
        return lower | (readByte(address + 1) << 8);
    }
    
    KIWI_FORCE_INLINE
    void writeWord(uint16_t address, uint16_t data) {
        writeByte(address + 1, (uint8_t)(data >> 8));
        writeByte(address, (uint8_t)(data & 0x00FF));
    }

    KIWI_FORCE_INLINE
    uint16_t popWord() {
        uint8_t lower = readByte(++SP);
        return lower | (readByte(++SP) << 8);
    }

    KIWI_FORCE_INLINE
    void pushWord(uint16_t data) {
        writeByte(SP--, (uint8_t)(data >> 8));
        writeByte(SP--, (uint8_t)(data & 0x00FF));
    }

}; // class Processor

} // namespace DMG_CPU
} // namespace kiwi

#endif // KIWI_DMG_CPU_HPP