#include <Kiwi/Log.hpp>

namespace kiwi {

void LogMessage(std::string_view tag, std::string_view message)
{
    fmt::print("({}) {}\n", tag, message);
}

} // namespace kiwi