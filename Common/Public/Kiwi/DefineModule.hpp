#ifndef KIWI_DEFINE_MODULE_HPP
#define KIWI_DEFINE_MODULE_HPP

#include <Kiwi/Config.hpp>

namespace kiwi {

struct ModuleDefinition
{
    const char * Name;
    bool (*Initialize)();
    void (*Terminate)();
};

} // namespace kiwi

#define KIWI_DEFINE_MODULE \
    extern "C" KIWI_API_EXPORT kiwi::ModuleDefinition _KiwiModule =

#endif // KIWI_DEFINE_MODULE_HPP