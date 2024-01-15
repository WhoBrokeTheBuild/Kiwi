#include "Emulator.hpp"

#include <Kiwi/Utility/Log.hpp>
#include <Kiwi/Utility/Exception.hpp>

#include <fstream>

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>

namespace kiwi {
namespace GameBoy {

Emulator::Emulator(MainWindow * mainWindow)
    : IEmulator(mainWindow)
    , _settingsWindow(new SettingsWindow())
{
    init(); // 
}

Emulator::~Emulator()
{
    // ?
    _renderer = nullptr;

    delete _settingsWindow;
}

void Emulator::init()
{
    DMG_CPU.reset();

    if (!_settingsWindow->bootstrapROM().isEmpty()) {
        DMG_CPU.loadBootstrapROM(_settingsWindow->bootstrapROM().toStdString());
    }
}

void Emulator::term()
{
    stopThread();
}

void Emulator::initWidgets()
{
    _menu = _mainWindow->menuBar()->addMenu("GameBoy");

    auto settingsWindowAction = _menu->addAction("&Settings");
    connect(settingsWindowAction, SIGNAL(triggered()), this, SLOT(showSettingsWindow()));

    auto loadCartridgeROMAction = _menu->addAction("&Load Cartridge ROM");
    connect(loadCartridgeROMAction, SIGNAL(triggered()), this, SLOT(loadCartridgeROM()));
}

void Emulator::termWidgets()
{
    _mainWindow->menuBar()->removeAction(_menu->menuAction());
}

void Emulator::doFrame()
{
    if (!_renderer) {
        return;
    }

    // TODO: Handle infinite loops?
    int lastLY = DMG_CPU.LY;
    for (;;) {
        DMG_CPU.tick();

        // TODO: IMPROVE
        // When the line wraps around
        if (DMG_CPU.LY == 0 && lastLY != DMG_CPU.LY) {
            break;
        }

        lastLY = DMG_CPU.LY;
    }

    // Copy image from DMG_CPU to Renderer2D

    const auto& backbuffer = _renderer->backbuffer();
    memcpy(backbuffer.data(), DMG_CPU.LCD, backbuffer.size());

    _renderer->nextImage();
}

QVulkanWindowRenderer * Emulator::createRenderer()
{
    delete _renderer;
    _renderer = new Renderer2D(this, { DMG_CPU::SCREEN_WIDTH, DMG_CPU::SCREEN_HEIGHT });

    return _renderer;
}

void Emulator::showSettingsWindow()
{
    _settingsWindow->show();
}

void Emulator::loadCartridgeROM()
{
    auto filename = QFileDialog::getOpenFileName(
        _mainWindow,
        "Load GameBoy Cartridge ROM",
        _settingsWindow->defaultROMDirectory(),
        "ROM files (*.gb *.gbc)"
    );

    if (!filename.isEmpty()) {
        Log(KIWI_ANCHOR, "Loading Cartridge from {}", filename.toStdString());

        DMG_CPU.loadCartridgeROM(filename.toStdString());
        init();
        
        play();
    }
}

KIWI_DEFINE_EMULATOR {
    .Name = "GameBoy",
    .CreateEmulator = 
        [](MainWindow * mainWindow) -> IEmulator * {
            return new Emulator(mainWindow);
        },
};

} // namespace GameBoy
} // namespace kiwi