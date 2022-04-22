#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Kiwi/Config.hpp>
#include <Kiwi/Containers.hpp>
#include <Kiwi/String.hpp>
#include <Kiwi/Emulator.hpp>

#include <QMainWindow>
#include <QVulkanWindow>

using namespace kiwi;

#if defined(KIWI_PLATFORM_WINDOWS)

    typedef HMODULE LibraryHandle;

#else

    typedef void * LibraryHandle;

#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow();

    ~MainWindow();
        
    void loadEmulator(const String& name);

    void freeEmulator();

private:

    QVulkanInstance _vkInstance;

    Emulator * _emulator = nullptr;

    LibraryHandle _emulatorLibrary = nullptr;

    EmulatorDefinition * _emulatorDefinition = nullptr;
    
};

#endif // MAINWINDOW_H
