#include <Kiwi/Module.hpp>
#include <Kiwi/DefineModule.hpp>
#include <Kiwi/Log.hpp>

#include <unordered_map>
#include <exception>

#if defined(KIWI_PLATFORM_WINDOWS)

    #include <Windows.h>

#else

    #include <dlfcn.h>

#endif

namespace kiwi {

#if defined(KIWI_PLATFORM_WINDOWS)

    typedef HMODULE ModuleHandle;

#else

    typedef void * ModuleHandle;

#endif

std::unordered_map<string, ModuleHandle> _Modules;

ModuleHandle _dlopen(const std::string& name)
{
    ModuleHandle handle = nullptr;

    #if defined(KIWI_PLATFORM_WINDOWS)

        handle = LoadLibraryA(name.c_str());
        if (!handle) {
            throw fmt::windows_error(GetLastError(), "Failed to load module '{}'", name);
        }

    #else
    
        string libName = "lib" + name;

        #if defined(KIWI_PLATFORM_APPLE)

            libName += ".dylib";

        #else

            libName += ".so";

        #endif

        // TODO: Investigate flags
        handle = dlopen(libName.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!handle) {
            throw std::runtime_error(fmt::format("Failed to load module '{}', {}", libName, dlerror()));
        }
    
    #endif

    return handle;
}

void * _dlsym(ModuleHandle handle, const std::string& symbol)
{
    #if defined(KIWI_PLATFORM_WINDOWS)

        return GetProcAddress(handle, symbol.c_str());

    #else

        return dlsym(handle, symbol.c_str());

    #endif
}

void _dlclose(ModuleHandle handle)
{
    #if defined(KIWI_PLATFORM_WINDOWS)

        FreeLibrary(handle);

    #else

        dlclose(handle);

    #endif
}

bool LoadModule(const std::string& name)
{
    ModuleHandle handle = _dlopen(name);
    if (!handle) {
        return false;
    }

    ModuleDefinition * definition = static_cast<ModuleDefinition *>(_dlsym(handle, "_KiwiModule"));
    if (!definition) {
        return false;
    }

    if (definition->Initialize) {
        if (!definition->Initialize()) {
            _dlclose(handle);
            
        }
    }
}

} // namespace kiwi