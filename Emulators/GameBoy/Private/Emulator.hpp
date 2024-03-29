#ifndef KIWI_GAMEBOY_EMULATOR_HPP
#define KIWI_GAMEBOY_EMULATOR_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/IEmulator.hpp>
#include <Kiwi/Renderer2D.hpp>
#include <Kiwi/Widgets/mainwindow.h>

#include <Kiwi/DMG-CPU/Processor.hpp>

#include <QMenu>

#include "Widgets/settingswindow.h"

namespace kiwi {
namespace GameBoy {

class Emulator : public IEmulator
{
    Q_OBJECT

public:

    DMG_CPU::Processor DMG_CPU;

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
        return { DMG_CPU::SCREEN_WIDTH * 6, DMG_CPU::SCREEN_HEIGHT * 6 };
    }

protected:

    QVulkanWindowRenderer * createRenderer() override;



public slots:

    void showSettingsWindow();

    void loadCartridgeROM();

private:

    Renderer2D * _renderer = nullptr;

    QMenu * _menu = nullptr;

    SettingsWindow * _settingsWindow = nullptr;

}; // class Emulator

} // namespace GameBoy
} // namespace kiwi

#endif // KIWI_GAMEBOY_EMULATOR_HPP