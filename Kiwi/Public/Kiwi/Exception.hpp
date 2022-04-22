#ifndef KIWI_EXCEPTION_HPP
#define KIWI_EXCEPTION_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/String.hpp>
#include <Kiwi/Log.hpp>

#include <stdexcept>
#include <utility>

namespace kiwi {

class KIWI_API RuntimeError : public std::runtime_error
{
public:

    KIWI_DISALLOW_COPY_AND_ASSIGN(RuntimeError)
    
    RuntimeError(String message)
        : std::runtime_error(message)
    { }

    template <class... Args>
    RuntimeError(StringView format, Args&&... args)
        : std::runtime_error(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)))
    { }

}; // RuntimeError

} // namespace kiwi

#endif // KIWI_EXCEPTION_HPP