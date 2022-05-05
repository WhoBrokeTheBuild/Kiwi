#include <Kiwi/Emulator.hpp>
#include <Kiwi/Log.hpp>

#include <chrono>
#include <typeinfo>

namespace kiwi {

using namespace std::chrono_literals;

Emulator::Emulator()
{
    _running = true;
    _thread = std::thread([this]() {
        run();
    });
}

Emulator::~Emulator()
{
    _running = false;
    _thread.join();
}

void Emulator::run()
{
    auto target = 1'000'000'000ns / targetFPS();

    while (_running) {
        auto before = std::chrono::high_resolution_clock::now();

        doFrame();

        auto after = std::chrono::high_resolution_clock::now();
        auto elapsed = after - before;
        auto difference = target - elapsed;

        if (difference.count() > 1'000'000) {
            std::this_thread::sleep_for(target - elapsed);
        }
        else {
            Log(KIWI_ANCHOR, "sleep_for cannot sleep for less than 1ms");
        }
    }
}

QVulkanWindowRenderer * Emulator::createRenderer()
{
    return nullptr;
}

} // namespace kiwi