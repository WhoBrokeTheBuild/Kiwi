#include "Emulator.hpp"

#include <Kiwi/Log.hpp>

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>

#include <QKeyEvent>
#include <QFileDialog>

namespace kiwi {
namespace CHIP8 {

Emulator::Emulator(QMainWindow * mainWindow)
    : IEmulator(mainWindow)
    , _renderer(new Renderer2D(this, { WIDTH, HEIGHT }))
{
}

Emulator::~Emulator()
{
    _renderer = nullptr;
}

void Emulator::initWidgets()
{
    QMenu * menu = new QMenu("CHIP-8");
    _menu = _mainWindow->menuBar()->addMenu(menu);

    auto loadProgramAction = menu->addAction("Load Program");
    connect(loadProgramAction, &QAction::triggered, this, &Emulator::loadProgram);
}

void Emulator::termWidgets()
{
    _mainWindow->menuBar()->removeAction(_menu);
}

void Emulator::reset()
{
    _loaded = false;

    memset(RAM, 0, sizeof(RAM));
    memset(Stack, 0, sizeof(Stack));

    I = 0x000;
    PC = 0x200;
    SP = 0;
    DelayTimer = 0;
    SoundTimer = 0;
    
    memset(V, 0, sizeof(V));

    WaitInputVx = -1;

    initFont();
}

void Emulator::doFrame()
{
    if (!_renderer || !_loaded) {
        return;
    }

    for (int i = 0; i < 20; ++i) {
        doStep();
    }

    // ?
    // frameReady();

    if (DelayTimer > 0) {
        --DelayTimer;
    }

    if (SoundTimer > 0) {
        --SoundTimer;
    }

    auto image = _renderer->image();
    for (size_t y = 0; y < HEIGHT; ++y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            size_t i = ((y * WIDTH) + x) * _renderer->components();
            uint8_t color = (_screen[y][x] ? 0xFF : 0x00);
            image[i + 0] = color;
            image[i + 1] = color;
            image[i + 2] = color;
        }
    }

    _renderer->nextImage();
}

void Emulator::doStep()
{
    if (!_loaded) {
        return;
    }

    uint8_t opcode1 = RAM[PC];
    uint8_t opcode2 = RAM[PC + 1];
    PC += 2;
    
    uint16_t opcode = (opcode1 << 8) | opcode2;

    unsigned W = (opcode1 & 0xF0) >> 4;
    unsigned X = (opcode1 & 0x0F);
    unsigned Y = (opcode2 & 0xF0) >> 4;
    unsigned Z = (opcode2 & 0x0F);

    unsigned NNN = ((opcode1 & 0x0F) << 8) | opcode2;
    auto & NN = opcode2;
    auto & N = Z;

    switch (W) {
    case 0x0:
        if (opcode2 == 0xE0) {
            // CLS
            memset(_screen, 0, sizeof(_screen));
        }
        else if (opcode2 == 0xEE) {
            // RET
            --SP;
            PC = Stack[SP];
        }
        else {
            // Machine Language Subroutines not supported
        }
        break;
    case 0x1: // 1NNN
        // JP NNN
        PC = NNN;
        break;
    case 0x2: // 2NNN
        // CALL NNN
        Stack[SP] = PC;
        ++SP;
        PC = NNN;
        break;
    case 0x3: // 3XNN
        // SE VX NN
        if (V[X] == NN) {
            PC += 2;
        }
        break;
    case 0x4: // 4XNN
        // SNE VX NN
        if (V[X] != NN) {
            PC += 2;
        }
        break;
    case 0x5: // 5XY0
        if (Z == 0) {
            // SE VX VY
            if (V[X] == V[Y]) {
                PC += 2;
            }
            break;
        }
    case 0x6: // 6XNN
        // LD VX NN
        V[X] = NN;
        break;
    case 0x7: // 7XNN
        // ADD VX NN
        V[X] += NN;
        break;
    case 0x8:
        if (Z == 0) { // 8XY0
            // LD VX VY
            V[X] = V[Y];
        }
        else if (Z == 1) { // 8XY1
            // OR VX VY
            V[X] |= V[Y];
        }
        else if (Z == 2) { // 8XY2
            // AND VX VY
            V[X] &= V[Y];
        }
        else if (Z == 3) { // 8XY3
            // XOR VX VY
            V[X] ^= V[Y];
        }
        else if (Z == 4) { // 8XY4
            // ADD VX VY
            if ((int)V[X] + (int)V[Y] > 0xFF) {
                V[0xF] = 1;
            }
            else {
                V[0xF] = 0;
            }

            V[X] += V[Y];
        }
        else if (Z == 5) { // 8XY5
            // SUB VX VY
            if ((int)V[X] - (int)V[Y] < 0) {
                V[0xF] = 0;
            }
            else {
                V[0XF] = 1;
            }

            V[X] -= V[Y];
        }
        else if (Z == 6) { // 8XY6
            // SHR VX VY
            V[0xF] = (V[Y] & 0x01);
            V[X] = (V[Y] >> 1);
        }
        else if (Z == 7) { // 8XY7
            // SUBN VX VY
            if ((int)V[Y] - (int)V[X] < 0) {
                V[0xF] = 0;
            }
            else {
                V[0xF] = 1;
            }

            V[X] = (V[Y] - V[X]);
        }
        else if (Z == 8) { // 8XYE
            // SHL VX VY
            V[0xF] = (V[Y] & 0x80) >> 7;
            V[X] = (V[Y] << 1);
        }
        break;
    case 0x9:
        if (Z == 0) {
            // SNE VX VY
            if (V[X] != V[Y]) {
                PC += 2;
            }
        }
        break;
    case 0xA: // ANNN
        // LD I NNN
        I = NNN;
        break;
    case 0xB: // BNNN
        // JP NNN + V0
        PC = NNN + V[0];
        break;
    case 0xC: // CXNN
        // RND VX % NN
        if (NN != 0) {
            V[X] = rand() % NN;
        }
        else {
            Log(KIWI_ANCHOR, "0xCXNN where NN == 0");
        }
        break;
    case 0xD: // DXYN
        // DRW VX VY N
        V[0xF] = 0;

        for (unsigned row = 0; row < N; ++row) {
            unsigned y = (V[Y] + row) % HEIGHT;
            uint8_t data = RAM[I + row];
            for (unsigned col = 0; col < 8; ++col) {
                unsigned x = (V[X] + col) % WIDTH;

                bool pixel = (data & (0x80 >> col));
                if (_screen[y][x] && pixel) {
                    V[0xF] = 1;
                }
                _screen[y][x] ^= pixel;
            }
        }
        break;
    case 0xE:
        if (opcode2 == 0x9E) { // EX9E
            // SKP VX
            if (_keys[V[X]]) {
                PC += 2;
            }
        }
        else if (opcode2 == 0xA1) { // EXA1
            // SKNP VX
            if (!_keys[V[X]]) {
                PC += 2;
            }
        }
    case 0xF:
        if (opcode2 == 0x07) { // FX07
            // LD VX DT
            V[X] = DelayTimer;
        }
        else if (opcode2 == 0x0A) { // FX0A
            // 
        }
        else if (opcode2 == 0x15) { // FX15
            // LD DT VX
            DelayTimer = V[X];
        }
        else if (opcode2 == 0x18) { // FX18
            // LD ST VX
            SoundTimer = V[X];
        }
        else if (opcode2 == 0x1E) { // FX1E
            // ADD I VX
            I += V[X];
        }
        else if (opcode2 == 0x29) { // FX29
            // LD I FONT[VX]
            I = 0x050 + (V[X] * 5);
        }
        else if (opcode2 == 0x33) { // FX33
            // BCD VX
            RAM[I] = V[X] / 100;
            RAM[I + 1] = (V[X] % 100) / 10;
            RAM[I + 2] = (V[X] % 10);
        }
        else if (opcode2 == 0x55) { // FX55
            // LD [I] V0..X
            for (unsigned i = 0; i < X; ++i) {
                RAM[I + i] = V[i];
            }

            I = I + X + 1;
        }
        else if (opcode2 == 0x65) { // FX65
            // LD V0..X [I]
            for (unsigned i = 0; i <= X; ++i) {
                V[i] = RAM[I + i];
            }

            I = I + X + 1;
        }
        break;
    default:
        // Unknown instruction
        break;
    }
}

QVulkanWindowRenderer * Emulator::createRenderer()
{
    return _renderer;
}
    
void Emulator::keyPressEvent(QKeyEvent * event)
{
    auto it = _keyMap.find((Qt::Key)event->key());
    if (it != _keyMap.end()) {
        _keys[it->second] = true;
    }
}

void Emulator::keyReleaseEvent(QKeyEvent * event)
{
    auto it = _keyMap.find((Qt::Key)event->key());
    if (it != _keyMap.end()) {
        _keys[it->second] = false;
    }
}

void Emulator::loadProgram()
{
    auto filename = QFileDialog::getOpenFileName(_mainWindow, "Load CHIP-8 Program", QString(), "*.ch8");
    if (!filename.isEmpty()) {
        std::ifstream file(filename.toStdString(), std::ios::binary | std::ios::ate);
        if (!file) {
            // TODO: Error
        }

        init();
        
        auto size = file.tellg();
        file.seekg(0);
        
        file.read((char *)RAM + 0x200, size);

        file.close();
        
        start();
    }
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
        [](QMainWindow * mainWindow) -> IEmulator * {
            return new Emulator(mainWindow);
        },
};

} // namespace CHIP8
} // namespace kiwi
