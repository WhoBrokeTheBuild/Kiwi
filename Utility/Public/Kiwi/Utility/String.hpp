#ifndef KIWI_UTILITY_String_HPP
#define KIWI_UTILITY_String_HPP

#include <Kiwi/Utility/Config.hpp>
#include <Kiwi/Utility/Containers.hpp>

#include <string>
#include <string_view>

namespace kiwi {
    
using namespace std::string_literals;

using String = std::string;

using StringView = std::string_view;

KIWI_UTILITY_API
bool StartsWith(StringView string, StringView prefix);

KIWI_UTILITY_API
bool EndsWith(StringView string, StringView suffix);

KIWI_UTILITY_API
List<String> Split(StringView string, String delim);

KIWI_UTILITY_API
String Join(List<String> stringList, String delim = {});

} // namespace kiwi

#endif // KIWI_UTILITY_String_HPP