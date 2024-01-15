#ifndef KIWI_ATARI2600_EMULATOR_HPP
#define KIWI_ATARI2600_EMULATOR_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/IEmulator.hpp>
#include <Kiwi/Renderer2D.hpp>
#include <Kiwi/Widgets/mainwindow.h>

#include <Kiwi/MOS6507/Processor.hpp>

#include <QMenu>

// #include "Widgets/settingswindow.h"

namespace kiwi {
namespace Atari2600 {

class Emulator : public IEmulator
{
    Q_OBJECT

public:

    MOS6507::Processor CPU;

    Emulator(MainWindow * mainWindow);

    virtual ~Emulator();

    void init() override;

    void term() override;

    void initWidgets() override;

    void termWidgets() override;

    void doFrame() override;

    unsigned targetFPS() override {
        return 60;
    }

    QSize initialSize() override {
        return { MOS6502::SCREEN_WIDTH * 6, MOS6502::SCREEN_HEIGHT * 6 };
    }

protected:

    QVulkanWindowRenderer * createRenderer() override;



public slots:

    void showSettingsWindow();

    void loadCartridgeROM();

private:

    Renderer2D * _renderer = nullptr;

    QMenu * _menu = nullptr;

    // SettingsWindow * _settingsWindow = nullptr;

}; // class Emulator

} // namespace GameBoy
} // namespace kiwi

#endif // KIWI_ATARI2600_EMULATOR_HPP