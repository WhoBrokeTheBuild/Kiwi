#include <Kiwi/Utility/String.hpp>

namespace kiwi {

KIWI_UTILITY_API
bool StartsWith(StringView string, StringView prefix)
{
    if (string.size() < prefix.size()) {
        return false;
    }

    return std::equal(
        prefix.begin(),
        prefix.end(),
        string.begin()
    );
}

KIWI_UTILITY_API
bool EndsWith(StringView string, StringView suffix)
{
    if (string.size() < suffix.size()) {
        return false;
    }

    return std::equal(
        suffix.begin(),
        suffix.end(),
        string.end() - suffix.size()
    );
}

KIWI_UTILITY_API
List<String> Split(StringView string, String delim)
{
    List<String> stringList;

    auto next = string.find(delim);
    while (next != String::npos) {
        stringList.push_back(String(string.substr(0, next)));
        string = string.substr(next + 1);
        next = string.find(delim);
    }

    return stringList;
}

KIWI_UTILITY_API
String Join(List<String> stringList, String delim /*= {}*/)
{
    size_t totalSize = 0;
    for (const auto& string : stringList) {
        totalSize += string.size() + delim.size();
    }

    String result;
    result.reserve(totalSize);

    for (const auto& string : stringList) {
        result += string;
        result += delim;
    }

    return result;
}

} // namespace kiwi