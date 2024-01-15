#ifndef KIWI_UTILITY_Log_HPP
#define KIWI_UTILITY_Log_HPP

#include <Kiwi/Utility/Config.hpp>
#include <Kiwi/Utility/String.hpp>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <fmt/os.h>
#include <fmt/color.h>

namespace kiwi {

inline StringView _LogGetFilename(StringView filename)
{
    size_t pivot = filename.find_last_of(KIWI_PATH_SEPARATOR);
    return (
        pivot == StringView::npos
        ? filename
        : filename.substr(pivot + 1)
    );
}

///
/// Evaluates to basename(__FILE__):__LINE__, used as the first argument to kiwi::Log()
///
#define KIWI_ANCHOR (fmt::format("{}:{}", kiwi::_LogGetFilename(__FILE__), __LINE__))

///
/// Log a message with a given tag.
///
/// @param tag The prefix to the log message, wrapped in (), usually KIWI_ANCHOR
/// @param message The message to log
///
KIWI_UTILITY_API
void LogMessage(StringView tag, StringView message);

///
/// Format and Log a message with a given tag.
///
/// @param tag The prefix to the log message, wrapped in (), usually KIWI_ANCHOR
/// @param format The format string for fmt::format()
/// @param args... The remaining arguments to fmt::format()
///
template <typename... Args>
KIWI_FORCE_INLINE void Log(StringView tag, StringView format, Args&&... args) {
    LogMessage(tag, fmt::vformat(format, fmt::make_format_args(args...)));
}

} // namespace kiwi

#if defined(KIWI_ENABLE_BENCHMARK)

    ///
    /// Initializes benchmarking within a scope. This cannot be run twice within the same scope.
    ///
    #define KIWI_BENCHMARK_START() \
        auto kiwiBenchmarkStart = std::chrono::high_resolution_clock::now()

    ///
    /// Finalizes benchmarking within a scope, and prints the results in milliseconds along with the 
    /// function name.
    ///
    #define KIWI_BENCHMARK_END() \
        kiwi::Log(KIWI_ANCHOR, "Function '{}' took {:.3} ms", KIWI_FUNCTION_NAME,                   \
            std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(                  \
                std::chrono::high_resolution_clock::now() - kiwiBenchmarkStart                      \
            ).count()                                                                               \
        )

#else

    #define KIWI_BENCHMARK_START() \
        do { } while (0)

    #define KIWI_BENCHMARK_END() \
        do { } while (0)

#endif

#endif // KIWI_UTILITY_Log_HPP