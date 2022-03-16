#ifndef KIWI_STRING_HPP
#define KIWI_STRING_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/Containers.hpp>

#include <string>
#include <string_view>

namespace kiwi {

using String = std::string;

using StringView = std::string_view;

KIWI_API
bool StartsWith(StringView string, StringView prefix);

KIWI_API
bool EndsWith(StringView string, StringView suffix);

KIWI_API
List<String> Split(StringView str, String delim);

KIWI_API
String Join(List<String> strList, String delim = {});

} // namespace kiwi

#endif // KIWI_STRING_HPP