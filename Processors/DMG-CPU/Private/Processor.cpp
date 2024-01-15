#include <Kiwi/DMG-CPU/Processor.hpp>
#include <Kiwi/Utility/Exception.hpp>

#include <cstdio>

namespace kiwi {
namespace DMG_CPU {

Processor::Processor()
{
}

Processor::~Processor()
{
    delete[] CartridgeROM;
}

void Processor::loadBootstrapROM(const String& filename)
{
    FILE * file = fopen(filename.c_str(), "rb");
    if (!file) {
        throw RuntimeError("Failed to open GameBoy Bootstrap ROM file '{}'", filename);
    }

    fseek(file, 0, SEEK_END);
    auto size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size != BOOTSTRAP_ROM_SIZE) {
        throw RuntimeError(
            "Invalid file size for GameBoy Bootstrap ROM. Expected {} bytes, found {} bytes",
            BOOTSTRAP_ROM_SIZE,
            size
        );
    }
    
    fread((char *)BootstrapROM, size, 1, file);
}

void Processor::loadCartridgeROM(const String& filename)
{
    FILE * file = fopen(filename.c_str(), "rb");
    if (!file) {
        throw RuntimeError("Failed to open GameBoy Cartridge ROM file '{}'", filename);
    }

    fseek(file, 0, SEEK_END);
    auto size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size % CARTRIDGE_ROM_BANK_SIZE != 0) {
        throw RuntimeError(
            "Invalid file size for GameBoy Cartridge ROM. Expected a multiple of {} bytes, found {} bytes",
            CARTRIDGE_ROM_BANK_SIZE,
            size
        );
    }

    delete[] CartridgeROM;
    CartridgeROM = new uint8_t[size / CARTRIDGE_ROM_BANK_SIZE][CARTRIDGE_ROM_BANK_SIZE];

    fread((char *)CartridgeROM, size, 1, file);
}

void Processor::reset()
{
    // General

    memset(LCD, 0, sizeof(LCD));

    CPUEnabled = true;
    PPUEnabled = true;
    SRAMEnabled = true;

    // Memory

    if (BootstrapLoaded) {
        BootstrapEnabled = true;
    }

    memset(VideoRAM, 0, sizeof(VideoRAM));
    VideoRAMBank = 0;

    memset(OAM, 0, sizeof(OAM));

    CartridgeROMBank = 0;

    memset(StaticRAM, 0, sizeof(StaticRAM));
    StaticRAMBank = 0;

    memset(WorkRAM, 0, sizeof(WorkRAM));
    WorkRAMBank = 0;

    memset(HighRAM, 0, sizeof(HighRAM));

    // Registers

    AF = 0x01B0;
    BC = 0x0013;
    DE = 0x00D8;
    HL = 0x014D;
    PC = (BootstrapLoaded ? 0x0000 : 0x0100);
    SP = 0xFFFE;
    IME = true;
    RequestIME = false;

    // Hardware I/O Registers

    IF.raw = 0x00;
    IE.raw = 0x00;
    JOYP.raw = 0x80; // TODO
    SCY = 0x00;
    SCX = 0x00;
    LY = 0x00;
    LYC = 0x00;
    WY = 0x00;
    WX = 0x00;
    BGP.raw = 0b11100100;
    OBP0.raw = 0b11100100;
    OBP1.raw = 0b11100100;
    LCDC.raw = 0x91;
    STAT.raw = 0x00;
    STAT.Mode = STAT_MODE_HBLANK;

    _lineTicks = 0;
}

void Processor::tick()
{
    tickCPU();

    tickPPU();

    tickAPU();
}

void Processor::tickAPU()
{

}

} // namespace DMG_CPU
} // namespace kiwi