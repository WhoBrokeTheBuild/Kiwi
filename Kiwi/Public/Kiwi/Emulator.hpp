#ifndef KIWI_EMULATOR_HPP
#define KIWI_EMULATOR_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/Math.hpp>
#include <Kiwi/String.hpp>

#include <QVulkanWindow>
#include <QMainWindow>

#include <atomic>
#include <thread>

namespace kiwi {
    
class KIWI_API Emulator : public QVulkanWindow
{
public:

    Emulator();

    virtual ~Emulator();

    virtual void start();

    virtual void stop();

    void setMainWindow(QMainWindow * mainWindow) {
        _mainWindow = mainWindow;
    }

    QMainWindow * mainWindow() const {
        return _mainWindow;
    }

    virtual bool loadROM(const String& filename) = 0;

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

    QMainWindow * _mainWindow = nullptr;

    std::thread _thread;

    std::atomic_bool _running = false;

};

struct KIWI_API EmulatorDefinition
{
    const char * Name;

    std::function<Emulator * (void)> CreateEmulator;
};

#define KIWI_EMULATOR_DEFINITION_SYMBOL _KiwiEmulatorDefinition

#define KIWI_DEFINE_EMULATOR \
    extern "C" KIWI_API_EXPORT kiwi::EmulatorDefinition KIWI_EMULATOR_DEFINITION_SYMBOL =

} // namespace kiwi

#endif // KIWI_EMULATOR_HPP