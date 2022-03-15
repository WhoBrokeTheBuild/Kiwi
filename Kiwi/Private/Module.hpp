#ifndef KIWI_MODULE_HPP
#define KIWI_MODULE_HPP

#include <Kiwi/Config.hpp>

namespace kiwi {

bool LoadModule(const std::string& name);

void FreeModule(const std::string& name);

void FreeAllModules();

} // namespace kiwi

#endif // KIWI_MODULE_HPP