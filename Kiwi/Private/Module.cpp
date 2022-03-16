#include <Kiwi/Module.hpp>
#include <Kiwi/DefineModule.hpp>
#include <Kiwi/Exception.hpp>

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

std::unordered_map<String, ModuleHandle> _ModuleMap;

ModuleHandle _dlopen(const String& name)
{
    ModuleHandle handle;

    #if defined(KIWI_PLATFORM_WINDOWS)

        handle = LoadLibraryA(name.c_str());
        if (!handle) {
            auto error = fmt::windows_error(GetLastError(), "Failed to load library '{}'", name);
            Log(KIWI_ANCHOR, "Failed to load library {}, {}", name, error.what());
        }

    #else
    
        String libName = "lib" + name;

        #if defined(KIWI_PLATFORM_APPLE)

            libName += ".dylib";

        #else

            libName += ".so";

        #endif

        handle = dlopen(libName.c_str(), RTLD_GLOBAL | RTLD_NOW);
        if (!handle) {
            Log(KIWI_ANCHOR, "Failed to load library {}, {}", libName, dlerror());
        }
    
    #endif

    return handle;
}

void * _dlsym(ModuleHandle handle, const String& symbol)
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

bool LoadModule(const String& name)
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

            Log(KIWI_ANCHOR, "Failed to initialize module {}", name);
            return false;
        }
    }

    _ModuleMap.emplace(name, handle);

    Log(KIWI_ANCHOR, "Loaded module {}", name);
    return true;
}

void UnloadModule(const String& name)
{
    auto it = _ModuleMap.find(name);
    if (it == _ModuleMap.end()) {
        Log(KIWI_ANCHOR, "Failed to unload module {}, module is not loaded", name);
        return;
    }

    ModuleHandle handle = it->second;
    ModuleDefinition * definition = static_cast<ModuleDefinition *>(_dlsym(handle, "_KiwiModule"));

    if (definition && definition->Terminate) {
        definition->Terminate();
    }

    _dlclose(handle);

    _ModuleMap.erase(it);

    Log(KIWI_ANCHOR, "Unloaded module {}", name);
}

void UnloadAllModules()
{
    for (auto it : _ModuleMap) {
        ModuleHandle handle = it.second;
        ModuleDefinition * definition = static_cast<ModuleDefinition *>(_dlsym(handle, "_KiwiModule"));

        if (definition && definition->Terminate) {
            definition->Terminate();
        }

        _dlclose(handle);

        Log(KIWI_ANCHOR, "Unloaded module {}", it.first);
    }

    _ModuleMap.clear();
}

} // namespace kiwi