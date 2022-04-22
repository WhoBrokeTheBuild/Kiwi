#include "mainwindow.hpp"

#include <Kiwi/Log.hpp>
#include <Kiwi/Exception.hpp>

#include <QMenuBar>
#include <QApplication>
#include <QLayout>
#include <QVulkanFunctions>

#include <nlohmann/json.hpp>

#if defined(KIWI_PLATFORM_WINDOWS)

    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>

#else

    #include <dlfcn.h>

#endif

MainWindow::MainWindow()
    : QMainWindow(nullptr)
{
    QMenu * fileMenu = menuBar()->addMenu("File");
    QMenu * emulatorMenu = fileMenu->addMenu("Emulator");

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

MainWindow::~MainWindow()
{
}

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
        
        String libName = "lib" + name;

        #if defined(KIWI_PLATFORM_APPLE)

            libName += ".dylib";

        #else

            libName += ".so";

        #endif

        _emulatorLibrary = dlopen(libName.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!_emulatorLibrary) {
            throw RuntimeError("Failed to load library '{}', {}", libName, dlerror());
        }

        _emulatorDefinition = (EmulatorDefinition *)dlsym(_emulatorLibrary, symbolName);
        if (!_emulatorDefinition) {
            throw RuntimeError("Failed to find {} symbol: {}", symbolName, dlerror());
        }

    #endif

    if (!_emulatorDefinition->CreateEmulator) {
        throw RuntimeError("Failed to load emulator '{}', no CreateEmulator function", _emulatorDefinition->Name);
    }

    Log(KIWI_ANCHOR, "Loading emulator '{}'", _emulatorDefinition->Name);

    _emulator = _emulatorDefinition->CreateEmulator();
    _emulator->setVulkanInstance(&_vkInstance);

    setCentralWidget(QWidget::createWindowContainer(_emulator));

    Log(KIWI_ANCHOR, "Loaded emulator '{}'", _emulatorDefinition->Name);
}

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
