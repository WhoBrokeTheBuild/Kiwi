#include <Kiwi/Utility/Log.hpp>
#include <Kiwi/Utility/Containers.hpp>

namespace kiwi {

KIWI_UTILITY_API
void LogMessage(StringView tag, StringView message)
{
    fmt::print("({}) {}\n", tag, message);
}

} // namespace kiwi