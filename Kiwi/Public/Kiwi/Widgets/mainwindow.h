#ifndef KIWI_WIDGETS_mainwindow_H
#define KIWI_WIDGETS_mainwindow_H

#include <Kiwi/Config.hpp>
#include <Kiwi/Utility/Containers.hpp>
#include <Kiwi/Utility/String.hpp>

#include <QMainWindow>
#include <QVulkanWindow>

namespace kiwi {

#if defined(KIWI_PLATFORM_WINDOWS)

    typedef HMODULE LibraryHandle;

#else

    typedef void * LibraryHandle;

#endif

class IEmulator;
class EmulatorDefinition;

class KIWI_API MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow();

    ~MainWindow();
        
    void loadEmulator(const String& name);

    void freeEmulator();

private slots:

    void playPauseTriggered();

    void resetTriggered();

    void showMenuBarTriggered(bool checked);

protected:

    void keyPressEvent(QKeyEvent * event) override;

    void keyReleaseEvent(QKeyEvent * event) override;

private:

    QVulkanInstance _vkInstance;

    IEmulator * _emulator = nullptr;

    LibraryHandle _emulatorLibrary = nullptr;

    EmulatorDefinition * _emulatorDefinition = nullptr;

    QAction * _playPauseAction = nullptr;

    QAction * _resetAction = nullptr;

    QAction * _exitAction = nullptr;
    
    QAction * _showMenuBarAction = nullptr;

};

} // namespace kiwi

#endif // KIWI_WIDGETS_mainwindow_H
