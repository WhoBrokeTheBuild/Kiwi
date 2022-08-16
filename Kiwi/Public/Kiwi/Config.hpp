#ifndef KIWI_Config_HPP
#define KIWI_Config_HPP

#include <Kiwi/Version.hpp>
#include <Kiwi/Utility/Macros.hpp>

#if defined(KIWI_EXPORT)

    #define KIWI_API KIWI_API_EXPORT

#else

    #define KIWI_API KIWI_API_IMPORT

#endif

#endif // KIWI_Config_HPP