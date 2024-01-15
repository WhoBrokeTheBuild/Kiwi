#include <Kiwi/IEmulator.hpp>

#include <Kiwi/Utility/Log.hpp>

#include <QKeyEvent>
#include <QCoreApplication>

#include <chrono>
#include <typeinfo>

namespace kiwi {

using namespace std::chrono_literals;

IEmulator::IEmulator(MainWindow * mainWindow)
    : _mainWindow(mainWindow)
{
    setFlags(QVulkanWindow::Flag::PersistentResources);
}

IEmulator::~IEmulator()
{
    stopThread();
}

void IEmulator::startThread()
{
    if (_isThreadRunning) {
        return;
    }

    _isThreadRunning = true;
    _thread = std::thread([this]() {
        run();
    });
}

void IEmulator::stopThread()
{
    _isThreadRunning = false;
    _thread.join();
}

void IEmulator::run()
{
    auto target = 1'000'000'000ns / targetFPS();

    while (_isThreadRunning) {
        auto before = std::chrono::high_resolution_clock::now();
        
        if (_isPlaying) {
            doFrame();
        }

        auto after = std::chrono::high_resolution_clock::now();
        auto elapsed = after - before;
        auto difference = target - elapsed;

        // Any less than 1ms and sleep_for isn't accurate
        if (difference.count() > 1'000'000) {
            std::this_thread::sleep_for(target - elapsed);
        }
        else {
            // Log(KIWI_ANCHOR, "sleep_for cannot sleep for less than 1ms");
        }
    }
}

void IEmulator::keyPressEvent(QKeyEvent * event)
{
    if (_mainWindow) {
        // TODO: Improve?
        QKeyEvent * copy = new QKeyEvent(event->type(), event->key(), event->modifiers(), event->text(), event->isAutoRepeat(), event->count());
        QCoreApplication::postEvent(_mainWindow, copy);
    }
}

void IEmulator::keyReleaseEvent(QKeyEvent * event)
{
    if (_mainWindow) {
        // TODO: Improve?
        QKeyEvent * copy = new QKeyEvent(event->type(), event->key(), event->modifiers(), event->text(), event->isAutoRepeat(), event->count());
        QCoreApplication::postEvent(_mainWindow, copy);
    }
}

} // namespace kiwi