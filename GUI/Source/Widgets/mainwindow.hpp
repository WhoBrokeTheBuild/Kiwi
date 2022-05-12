#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/Containers.hpp>
#include <Kiwi/String.hpp>
#include <Kiwi/Emulator.hpp>

#include <QMainWindow>
#include <QVulkanWindow>

#include "autohidingmenubar.hpp"

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

private slots:

    void showMenuBarChanged(bool checked);

protected:

    void keyPressEvent(QKeyEvent * event) override;

private:

    QVulkanInstance _vkInstance;

    Emulator * _emulator = nullptr;

    LibraryHandle _emulatorLibrary = nullptr;

    EmulatorDefinition * _emulatorDefinition = nullptr;

    AutoHidingMenuBar * _menuBar = nullptr;
    
    QAction * _showMenuBarAction = nullptr;

};

#endif // MAINWINDOW_HPP
