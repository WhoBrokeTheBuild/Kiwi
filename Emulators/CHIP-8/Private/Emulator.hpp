#ifndef KIWI_CHIP8_EMULATOR_HPP
#define KIWI_CHIP8_EMULATOR_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/IEmulator.hpp>
#include <Kiwi/Renderer2D.hpp>

#include <QMenuBar>

#include <cstdint>
#include <memory>

namespace kiwi {
namespace CHIP8 {

class Emulator : public IEmulator
{
public:

    uint8_t RAM[4096];

    uint16_t I;

    uint16_t PC;

    uint8_t SP;

    uint8_t DelayTimer;

    uint8_t SoundTimer;

    uint8_t V[16];

    uint16_t Stack[16];

    int WaitInputVx;

    Emulator(QMainWindow * mainWindow);

    virtual ~Emulator();

    void init() override;

    void term() override;

    void initWidgets() override;

    void termWidgets() override;

    bool loadROM(const String& filename);

    void doStep();

    void doFrame() override;
    
    unsigned targetFPS() override {
        return 30;
    }

    QSize initialSize() override {
        return { WIDTH * 10, HEIGHT * 10 };
    };

protected:

    QVulkanWindowRenderer * createRenderer() override;

    void keyPressEvent(QKeyEvent * event) override;

    void keyReleaseEvent(QKeyEvent * event) override;

public slots:

    void loadProgram();

private:

    static const size_t WIDTH = 64;
    static const size_t HEIGHT = 32;

    bool _screen[HEIGHT][WIDTH] = { false };

    bool _keys[16] = { false };

    // Original            Mapped
    // 1 | 2 | 3 | C       1 | 2 | 3 | 4
    // 4 | 5 | 6 | D       Q | W | E | R
    // 7 | 8 | 9 | E       A | S | D | F
    // A | 0 | B | F       Z | X | C | V

    Map<Qt::Key, uint8_t> _keyMap = {
        { Qt::Key_1,     0x1 },
        { Qt::Key_2,     0x2 },
        { Qt::Key_3,     0x3 },
        { Qt::Key_4,     0xC },

        { Qt::Key_Q,     0x4 },
        { Qt::Key_W,     0x5 },
        { Qt::Key_E,     0x6 },
        { Qt::Key_R,     0xD },

        { Qt::Key_A,     0x7 },
        { Qt::Key_S,     0x8 },
        { Qt::Key_D,     0x9 },
        { Qt::Key_F,     0xE },

        { Qt::Key_Z,     0xA },
        { Qt::Key_X,     0x0 },
        { Qt::Key_C,     0xB },
        { Qt::Key_V,     0xF },

        { Qt::Key_Up,    0x5 },
        { Qt::Key_Down,  0x8 },
        { Qt::Key_Left,  0x7 },
        { Qt::Key_Right, 0x9 },
    };

    Renderer2D * _renderer = nullptr;

    QAction * _menu = nullptr;

    QList<QAction *> _ipfOptions;

    void initFont();

}; // class Emulator

} // namespace CHIP8
} // namespace kiwi

#endif // KIWI_CHIP8_EMULATOR_HPP