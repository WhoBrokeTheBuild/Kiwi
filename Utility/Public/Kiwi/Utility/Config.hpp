#ifndef KIWI_UTILITY_Config_HPP
#define KIWI_UTILITY_Config_HPP

#include <Kiwi/Utility/Macros.hpp>

#if defined(KIWI_UTILITY_EXPORT)

    #define KIWI_UTILITY_API KIWI_API_EXPORT

#else

    #define KIWI_UTILITY_API KIWI_API_IMPORT

#endif

#endif // KIWI_Config_HPP

