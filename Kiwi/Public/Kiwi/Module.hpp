#ifndef KIWI_MODULE_HPP
#define KIWI_MODULE_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/String.hpp>

namespace kiwi {

bool LoadModule(const String& name);

void UnloadModule(const String& name);

void UnloadAllModules();

} // namespace kiwi

#endif // KIWI_MODULE_HPP