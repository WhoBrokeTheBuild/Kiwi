#ifndef KIWI_UTILITY_Macros_HPP
#define KIWI_UTILITY_Macros_HPP

///
/// Platform
///

#if defined(_WIN32)

    #define KIWI_PLATFORM_WINDOWS 1

#elif defined(__APPLE__)

    #define KIWI_PLATFORM_APPLE 1

    #include "TargetConditionals.h"

    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

        // Apple iOS
        #define KIWI_PLATFORM_IOS 1

    #elif TARGET_OS_MAC

        // Apple macOS/OSX
        #define KIWI_PLATFORM_MACOS 1

    #endif

#elif defined(__unix__)

    #define KIWI_PLATFORM_UNIX 1

    #if defined(__linux__)

        #define KIWI_PLATFORM_LINUX 1

    #endif

    #if defined(__ANDROID__)

        #define KIWI_PLATFORM_ANDROID 1

    #endif

#elif defined(__EMSCRIPTEN__)

    #define KIWI_PLATFORM_EMSCRIPTEN 1

#else

    #warning Unsupported Platform

#endif

///
/// Compiler
///

#if defined(_MSC_VER)

    // Microsoft VisualStudio C/C++
    #define KIWI_COMPILER_MSVC _MSC_VER

#elif defined(__clang__)

    // Apple Clang Compiler
    #define KIWI_COMPILER_CLANG __clang__

#elif defined(__INTEL_COMPILER)

    // Intel Compiler
    #define KIWI_COMPILER_ICC __INTEL_COMPILER

#elif defined(__GNUC__)
    
    // GNU Compiler Collection
    #define KIWI_COMPILER_GCC __GNUC__

#else

    #warning Unsupported Compiler

#endif

///
/// Build
///

#if defined(NDEBUG)

    #define KIWI_BUILD_RELEASE 1

#else

    #define KIWI_BUILD_DEBUG 1

#endif

///
/// Utility
///

#if KIWI_COMPILER_MSVC

    #define KIWI_FUNCTION_NAME __FUNCSIG__

#elif KIWI_COMPILER_GCC || KIWI_COMPILER_CLANG

    #define KIWI_FUNCTION_NAME __PRETTY_FUNCTION__

#else

    #define KIWI_FUNCTION_NAME __func__

#endif

#if KIWI_PLATFORM_WINDOWS

    #define KIWI_PATH_SEPARATOR "\\"
    #define KIWI_PATH_LIST_SEPARATOR ";"

#else

    #define KIWI_PATH_SEPARATOR "/"
    #define KIWI_PATH_LIST_SEPARATOR ":"

#endif

#if KIWI_COMPILER_MSVC

    #define KIWI_PACK(DECL) \
        __pragma(pack(push, 1))                                                                     \
        DECL                                                                                        \
        __pragma(pack(pop))

#else

    #define KIWI_PACK(DECL) \
        DECL __attribute__((__packed__))

#endif

#if KIWI_COMPILER_MSVC

    #define KIWI_FORCE_INLINE __forceinline

#else 

    #define KIWI_FORCE_INLINE __attribute__((always_inline)) inline

#endif

#if KIWI_COMPILER_MSVC

    #define KIWI_DISABLE_WARNINGS() \
        __pragma(warning(push, 0))
    
    #define KIWI_ENABLE_WARNINGS() \
        __pragma(warning(pop))

#elif KIWI_COMPILER_CLANG

    #define KIWI_DISABLE_WARNINGS() \
        _Pragma("clang diagnostic push") \
        _Pragma("clang diagnostic ignored \"-Wall\"")
    
    #define KIWI_ENABLE_WARNINGS() \
        _Pragma("clang diagnostic pop")

#elif KIWI_COMPILER_GCC

    #define KIWI_DISABLE_WARNINGS() \
        _Pragma("GCC diagnostic push") \
        _Pragma("GCC diagnostic ignored \"-Wall\"")
    
    #define KIWI_ENABLE_WARNINGS() \
        _Pragma("GCC diagnostic pop")

#else

    #define KIWI_DISABLE_WARNINGS()

    #define KIWI_ENABLE_WARNINGS()

#endif

#define KIWI_UNUSED(V) (void)(V)

#define _KIWI_STRINGIFY(STR) #STR
#define KIWI_STRINGIFY(STR) _KIWI_STRINGIFY(STR)

///
/// Shared Library Configuration
///

#if KIWI_COMPILER_MSVC

    #define KIWI_API_EXPORT __declspec(dllexport)
    #define KIWI_API_IMPORT __declspec(dllimport)

#elif KIWI_COMPILER_GCC || KIWI_COMPILER_CLANG

    #define KIWI_API_EXPORT __attribute__ ((__visibility__ ("default")))
    #define KIWI_API_IMPORT __attribute__ ((__visibility__ ("default")))

#endif

#endif // KIWI_UTILITY_Macros_HPP