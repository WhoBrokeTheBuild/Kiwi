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
    delete CartridgeROM;
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

    delete CartridgeROM;
    CartridgeROM = new uint8_t[size / CARTRIDGE_ROM_BANK_SIZE][CARTRIDGE_ROM_BANK_SIZE];

    fread((char *)CartridgeROM, size, 1, file);
}

void Processor::reset()
{
    A = 0x01;
    F = 0x0B;
    PC = 0x0150;
    
    memset(HighRAM, 0, sizeof(HighRAM));
    
    if (BootstrapROM) {
        BootstrapEnabled = true;
        PC = 0;
    }

    CartridgeROMBank = 0;
}

void Processor::tick()
{
    tickCPU();
    tickPPU();
    tickAPU();
}

void Processor::tickPPU()
{

}

void Processor::tickAPU()
{

}

} // namespace DMG_CPU
} // namespace kiwi