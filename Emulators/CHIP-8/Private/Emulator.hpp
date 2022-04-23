#ifndef KIWI_CHIP8_EMULATOR_HPP
#define KIWI_CHIP8_EMULATOR_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/Emulator.hpp>
#include <Kiwi/Renderer2D.hpp>

#include <QTimer>

#include <cstdint>
#include <memory>

namespace kiwi {
namespace CHIP8 {

class Emulator : public kiwi::Emulator
{
public:

    uint8_t RAM[4096];

    uint16_t I;

    uint16_t PC;

    uint16_t SP;

    uint8_t DelayTimer;

    uint8_t SoundTimer;

    uint8_t V[0xF];

    Emulator();

    virtual ~Emulator();

    void doStep();

    void doFrame() override;

protected:

    QVulkanWindowRenderer * createRenderer() override;

private:

    static const size_t WIDTH = 64;
    static const size_t HEIGHT = 32;

    bool _screen[WIDTH][HEIGHT];

    bool _keys[0xF] = { false };

    Renderer2D * _renderer = nullptr;

    QTimer _timer;

    void initFont();

}; // class Emulator

} // namespace CHIP8
} // namespace kiwi

#endif // KIWI_CHIP8_EMULATOR_HPP