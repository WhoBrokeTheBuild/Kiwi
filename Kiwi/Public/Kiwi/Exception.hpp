#ifndef KIWI_EXCEPTION_HPP
#define KIWI_EXCEPTION_HPP

#include <Kiwi/String.hpp>
#include <Kiwi/Log.hpp>

#include <stdexcept>
#include <utility>

namespace kiwi {

class KIWI_API RuntimeError : public std::runtime_error
{
public:
    
    RuntimeError(String message)
        : std::runtime_error(message)
    { }

    template <class... Args>
    RuntimeError(StringView format, Args&&... args)
        : std::runtime_error(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)))
    { }


};

#if defined(KIWI_PLATFORM_WINDOWS)

    using WindowsError = fmt::windows_error;

#endif

} // namespace kiwi

#endif // KIWI_EXCEPTION_HPP