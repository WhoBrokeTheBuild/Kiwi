#include <Kiwi/Log.hpp>
#include <Kiwi/Containers.hpp>

namespace kiwi {

KIWI_API
void LogMessage(std::string_view tag, std::string_view message)
{
    fmt::print("({}) {}\n", tag, message);
}

} // namespace kiwi