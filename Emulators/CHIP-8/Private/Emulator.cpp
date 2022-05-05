#include "Emulator.hpp"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>

namespace kiwi {
namespace CHIP8 {

Emulator::Emulator()
    : _renderer(new Renderer2D(this, { WIDTH, HEIGHT }))
{
    setFlags(QVulkanWindow::Flag::PersistentResources);

    reset();

    // Test image data
    auto image = _renderer->image();
    for (size_t i = 0; i < image.size(); i += 4) {
        image[i + 0] = rand() % 256;
        image[i + 1] = rand() % 256;
        image[i + 2] = rand() % 256;
    }
}

Emulator::~Emulator()
{
    _renderer = nullptr;
}

void Emulator::reset()
{
    memset(RAM, 0, sizeof(RAM));

    I = 0x000;
    PC = 0x200;
    SP = 0x000;
    DelayTimer = 0;
    SoundTimer = 0;
    
    memset(V, 0, sizeof(V));

    WaitInputVx = -1;

    initFont();
}

bool Emulator::loadROM(const String& filename)
{
    reset();

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        return false;
    }

    auto size = file.tellg();
    file.read((char *)RAM + 0x200, size);

    file.close();
    
    return true;
}

void Emulator::doStep()
{
    return;

    uint8_t opcode1 = RAM[PC];
    uint8_t opcode2 = RAM[PC + 1];
    PC += 2;

    unsigned NNN = ((opcode1 & 0x0F) << 4) | opcode2;
    
    unsigned W = (opcode1 & 0xF0) >> 4;
    unsigned X = (opcode1 & 0x0F);
    unsigned Y = (opcode2 & 0xF0) >> 4;
    unsigned Z = (opcode2 & 0x0F);

#define PUSH(X)

    switch (W) {
    case 0x0:
        if (opcode2 == 0xE0) {
            // Clear
        }
        else if (opcode2 == 0xEE) {
            // Return
            PC = (RAM[SP] << 8) | RAM[SP + 1];
            SP += 2;
        }
        else {
            // Machine Language Subroutines not supported
        }
        break;
    case 0x1: // 1NNN
        // Jump
        PC = NNN;
        break;
    case 0x2: // 2NNN
        // Call
        SP -= 2;
        RAM[SP] = ((PC & 0x0F00) >> 8);
        RAM[SP + 1] = (PC & 0x00FF);
        PC = NNN;
        break;
    case 0x3: // 3XNN
        // if VX != NN
        if (V[X] == opcode2) {
            PC += 2;
        }
        break;
    case 0x4: // 4XNN
        // if VX == NN
        if (V[X] != opcode2) {
            PC += 2;
        }
        break;
    case 0x5: // 5XY0
        if (Z == 0) {
            // if CX != VY
            if (V[X] == V[Y]) {
                PC += 2;
            }
        }
        break;
    case 0x6: // 6XNN
        // VX == NN
        V[X] += opcode2;
        break;
    case 0x7: // 7XNN
        // VX += NN
        V[X] += opcode2;
        break;
    case 0x8:
        if (Z == 0) { // 8XY0
            V[X] = V[Y];
        }
        else if (Z == 1) { // 8XY1
            V[X] |= V[Y];
        }
        else if (Z == 2) { // 8XY2
            V[X] &= V[Y];
        }
        else if (Z == 3) { // 8XY3
            V[X] ^= V[Y];
        }
        else if (Z == 4) { // 8XY4
            if ((int)V[X] + (int)V[Y] > 0xFF) {
                V[0xF] = 1;
            }
            else {
                V[0xF] = 0;
            }

            V[X] += V[Y];
        }
        else if (Z == 5) { // 8XY5
            if ((int)V[X] - (int)V[Y] < 0) {
                V[0xF] = 0;
            }
            else {
                V[0XF] = 1;
            }

            V[X] -= V[Y];
        }
        else if (Z == 6) { // 8XY6
            V[0xF] = (V[Y] & 0x01);
            V[X] = (V[Y] >> 1);
        }
        else if (Z == 7) { // 8XY7
            if ((int)V[Y] - (int)V[X] < 0) {
                V[0xF] = 0;
            }
            else {
                V[0xF] = 1;
            }

            V[X] = (V[Y] - V[X]);
        }
        else if (Z == 8) { // 8XYE
            V[0xF] = (V[Y] & 0x80) >> 7;
            V[X] = (V[Y] << 1);
        }
        break;
    case 0x9:
        if (Z == 0) {
            // if VX == VY
            if (V[X] != V[Y]) {
                PC += 2;
            }
        }
        break;
    case 0xA: // ANNN
        I = NNN;
        break;
    case 0xB: // BNNN
        PC = NNN + V[0];
        break;
    case 0xC: // CXNN
        V[X] = rand() % opcode2;
        break;
    case 0xD: // DXYN
        // Draw
        {
            unsigned x = (V[X] % WIDTH);
            unsigned y = (V[Y] % HEIGHT);
            V[0xF] = 0;

            for (unsigned row = 0; row < Z; ++row) {
                if (y + row >= HEIGHT) {
                    break;
                }

                for (unsigned col = 0; col < 8; ++col) {
                    if (x + col >= WIDTH) {
                        break;
                    }

                    bool bit = ((RAM[I + row] >> col) & 0x01);

                    if (_screen[x][y] && bit) {
                        _screen[x][y] = false;
                        V[0xF] = 1;
                    }
                    else {
                        _screen[x][y] = bit;
                    }
                }
            }
        }
        break;
    case 0xE:
        if (opcode2 == 0x9E) { // EX9E
            if (_keys[V[X]]) {
                PC += 2;
            }
        }
        else if (opcode2 == 0xA1) { // EXA1
            if (!_keys[V[X]]) {
                PC += 2;
            }
        }
        break;
    case 0xF:
        if (opcode2 == 0x07) { // FX07
            V[X] = DelayTimer;
        }
        else if (opcode2 == 0x0A) { // FX0A
            // 
        }
        else if (opcode2 == 0x15) { // FX15
            DelayTimer = V[X];
        }
        else if (opcode2 == 0x18) { // FX18
            SoundTimer = V[X];
        }
        else if (opcode2 == 0x1E) { // FX1E
            I += V[X];
        }
        else if (opcode2 == 0x29) { // FX29
            I = 0x050 + (V[X] * 5);
        }
        else if (opcode2 == 0x33) { // FX33
            RAM[I] = V[X] / 100;
            RAM[I + 1] = (V[X] % 100) / 10;
            RAM[I + 2] = (V[X] % 10);
        }
        else if (opcode2 == 0x55) { // FX55
            for (unsigned i = 0; i < X; ++i) {
                RAM[I + i] = V[i];
            }

            I = I + X + 1;
        }
        else if (opcode2 == 0x65) { // FX65
            for (unsigned i = 0; i < X; ++i) {
                V[i] = RAM[I + i];
            }

            I = I + X + 1;
        }
        break;
    }
}

void Emulator::doFrame()
{
    // for (int i = 0; i < 100; ++i) {
        doStep();
    // }

    // ?
    // frameReady();

    if (DelayTimer > 0) {
        --DelayTimer;
    }

    if (SoundTimer > 0) {
        --SoundTimer;
    }
}

QVulkanWindowRenderer * Emulator::createRenderer()
{
    return _renderer;
}

void Emulator::initFont()
{
    const uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    memcpy(&RAM[0x050], font, sizeof(font));
}

KIWI_DEFINE_EMULATOR {
    .Name = "CHIP-8",
    .CreateEmulator = 
        []() -> kiwi::Emulator * {
            return new Emulator();
        },
};

} // namespace CHIP8
} // namespace kiwi
