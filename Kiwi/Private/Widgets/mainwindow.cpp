#include <Kiwi/Widgets/mainwindow.h>

#include <Kiwi/IEmulator.hpp>
#include <Kiwi/Utility/Log.hpp>
#include <Kiwi/Utility/Exception.hpp>

#include <QApplication>
#include <QKeyEvent>
#include <QLayout>
#include <QMenuBar>
#include <QVulkanFunctions>

#include <nlohmann/json.hpp>

#if defined(KIWI_PLATFORM_WINDOWS)

    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>

#else

    #include <dlfcn.h>

#endif

namespace kiwi {

KIWI_API
MainWindow::MainWindow()
    : QMainWindow(nullptr)
{
    QMenu * emulatorMenu = menuBar()->addMenu("&Emulator");

    _playPauseAction = emulatorMenu->addAction("&Play/Pause");
    connect(_playPauseAction, SIGNAL(triggered()), this, SLOT(playPauseTriggered()));

    _resetAction = emulatorMenu->addAction("&Reset");
    connect(_resetAction, SIGNAL(triggered()), this, SLOT(resetTriggered()));

    // _exitAction = emulatorMenu->addAction("E&xit");
    // connect(_exitAction, SIGNAL(triggered()), &app, SLOT(quit()));

    QMenu * viewMenu = menuBar()->addMenu("&View");
    _showMenuBarAction = viewMenu->addAction("Show &Menu Bar");
    _showMenuBarAction->setCheckable(true);
    _showMenuBarAction->setChecked(true);
    connect(_showMenuBarAction, SIGNAL(triggered(bool)), this, SLOT(showMenuBarTriggered(bool)));

    _vkInstance.setExtensions(
        QByteArrayList()
        << VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    );

    _vkInstance.setLayers(
        QByteArrayList()
        << "VK_LAYER_KHRONOS_validation"
    );

    if (!_vkInstance.create()) {
        qFatal("Failed to create Vulkan Instance: %d", _vkInstance.errorCode());
    }
}

KIWI_API
MainWindow::~MainWindow()
{
}

KIWI_API
void MainWindow::loadEmulator(const String& name)
{
    freeEmulator();

    const char symbolName[] = KIWI_STRINGIFY(KIWI_EMULATOR_DEFINITION_SYMBOL);

    Log(KIWI_ANCHOR, "Loading emulator from '{}'", name);

    #if defined(KIWI_PLATFORM_WINDOWS)

        _emulatorLibrary = LoadLibraryA(name.c_str());
        if (!_emulatorLibrary) {
            throw fmt::windows_error(GetLastError(), "Failed to load library '{}'", name);
        }
        
        _emulatorDefinition = (EmulatorDefinition *)GetProcAddress(_emulatorLibrary, symbolName);
        if (!_emulatorDefinition) {
            throw fmt::windows_error(GetLastError(), "Failed to find symbol '{}'", symbolName);
        }

    #else

        // TODO: Check for full filepath
        
        _emulatorLibrary = dlopen(name.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!_emulatorLibrary) {
            throw RuntimeError("Failed to load library '{}', {}", name, dlerror());
        }

        _emulatorDefinition = (EmulatorDefinition *)dlsym(_emulatorLibrary, symbolName);
        if (!_emulatorDefinition) {
            throw RuntimeError("Failed to find {} symbol: {}", symbolName, dlerror());
        }

    #endif

    if (!_emulatorDefinition->CreateEmulator) {
        throw RuntimeError("Failed to load emulator '{}', no CreateEmulator function found", _emulatorDefinition->Name);
    }

    Log(KIWI_ANCHOR, "Loading emulator '{}'", _emulatorDefinition->Name);

    _emulator = _emulatorDefinition->CreateEmulator(this);
    _emulator->setVulkanInstance(&_vkInstance);
    _emulator->initWidgets();
    
    _emulator->startThread();

    setCentralWidget(QWidget::createWindowContainer(_emulator, this));

    resize(_emulator->initialSize() + QSize(0, menuBar()->height()));

    Log(KIWI_ANCHOR, "Loaded emulator '{}'", _emulatorDefinition->Name);
}

KIWI_API
void MainWindow::freeEmulator()
{
    delete _emulator;
    _emulator = nullptr;

    if (!_emulatorLibrary) {
        return;
    }

    #if defined(KIWI_PLATFORM_WINDOWS)

        FreeLibrary(_emulatorLibrary);

    #else

        dlclose(_emulatorLibrary);

    #endif

    _emulatorLibrary = nullptr;
    _emulatorDefinition = nullptr;
    
    layout()->removeWidget(centralWidget());
}

KIWI_API
void MainWindow::playPauseTriggered()
{
    if (_emulator) {
        if (_emulator->isPlaying()) {
            _emulator->pause();
        }
        else {
            _emulator->play();
        }
    }
}

KIWI_API
void MainWindow::resetTriggered()
{
    if (_emulator) {
        // _emulator->reset();
    }
}

KIWI_API
void MainWindow::showMenuBarTriggered(bool checked)
{
    // _menuBar->setAutoHiding(!checked);
}

KIWI_API
void MainWindow::keyPressEvent(QKeyEvent * event)
{
    
}

KIWI_API
void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
    // if (event->key() == Qt::Key_Alt) {
    //     _menuBar->peek();
    // }
}

} // namespace kiwi