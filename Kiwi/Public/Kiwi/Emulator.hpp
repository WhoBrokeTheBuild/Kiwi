#ifndef KIWI_EMULATOR_HPP
#define KIWI_EMULATOR_HPP

#include <Kiwi/Config.hpp>

#include <QVulkanWindow>

namespace kiwi {
    
class KIWI_API Emulator : public QVulkanWindow
{
public:

    Emulator();

    virtual ~Emulator();

    virtual void doFrame() { }

protected:

    virtual QVulkanWindowRenderer * createRenderer() = 0;

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