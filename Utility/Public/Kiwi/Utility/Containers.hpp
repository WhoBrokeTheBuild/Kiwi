#ifndef KIWI_UTILITY_Containers_HPP
#define KIWI_UTILITY_Containers_HPP

#include <Kiwi/Utility/Config.hpp>

#include <algorithm>
#include <array>
#include <deque>
#include <list>
#include <set>
#include <span>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace kiwi {

template <typename ValueType, size_t Size>
using Array = std::array<ValueType, Size>;

template <typename ValueType>
using Queue = std::deque<ValueType>;

template <typename ValueType>
using Set = std::set<ValueType>;

template <typename ValueType, size_t Size = std::dynamic_extent>
using Span = std::span<ValueType, Size>;

template <typename... ValueTypeList>
using Tuple = std::tuple<ValueTypeList...>;

template <typename KeyType, typename ValueType>
using Map = std::unordered_map<KeyType, ValueType>;

template <typename ValueType>
using List = std::vector<ValueType>;

template <typename ValueType>
using LinkedList = std::list<ValueType>;

template <typename ContainerType, typename ValueType>
concept HasFindMethod = requires(ContainerType container, ValueType value) {
    container.find(value);
};

template <typename ContainerType, typename ValueType>
inline auto Find(ContainerType&& haystack, ValueType&& needle)
{
    if constexpr (HasFindMethod<ContainerType, ValueType>) {
        return haystack.find(needle);
    }
    else {
        return std::find(std::begin(haystack), std::end(haystack), needle);
    }
}

template <typename ContainerType, typename ValueType>
inline bool Contains(ContainerType&& haystack, ValueType&& needle)
{
    return (Find(haystack, needle) != haystack.end());
}

} // namespace kiwi

#endif // KIWI_UTILITY_Containers_HPP