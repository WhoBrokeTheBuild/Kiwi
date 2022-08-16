#ifndef KIWI_IEmulator_HPP
#define KIWI_IEmulator_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/Utility/Math.hpp>
#include <Kiwi/Utility/String.hpp>
#include <Kiwi/Widgets/mainwindow.h>

#include <QVulkanWindow>

#include <atomic>
#include <thread>

namespace kiwi {

class KIWI_API IEmulator : public QVulkanWindow
{
public:

    IEmulator(MainWindow * mainWindow);

    virtual ~IEmulator();

    virtual void init() { }

    virtual void term() { }

    virtual void play() {
        _isPlaying = true;
    }

    virtual void pause() {
        _isPlaying = false;
    }

    virtual bool isPlaying() const {
        return _isPlaying;
    }

    virtual void startThread();

    virtual void stopThread();

    virtual bool isThreadRunning() const {
        return _isThreadRunning;
    }

    virtual void initWidgets() { }

    virtual void termWidgets() { }

    // bool isLoaded() const { return _loaded; }

    virtual void doFrame() = 0;

    virtual unsigned targetFPS() {
        return 60;
    }

    virtual QSize initialSize() {
        return { 640, 480 };
    }

protected:

    virtual void run();

    void keyPressEvent(QKeyEvent * event) override;

    void keyReleaseEvent(QKeyEvent * event) override;

    MainWindow * _mainWindow = nullptr;

    std::thread _thread;

    std::atomic_bool _isThreadRunning = false;

    std::atomic_bool _isPlaying = false;

    bool _isLoaded = false; // ?

    size_t _instructionLogIndex;

};

struct KIWI_API EmulatorDefinition
{
    const char * Name;

    std::function<IEmulator * (MainWindow*)> CreateEmulator;
};

#define KIWI_EMULATOR_DEFINITION_SYMBOL _KiwiEmulatorDefinition

#define KIWI_DEFINE_EMULATOR \
    extern "C" KIWI_API_EXPORT kiwi::EmulatorDefinition KIWI_EMULATOR_DEFINITION_SYMBOL =

} // namespace kiwi

#endif // KIWI_IEmulator_HPP