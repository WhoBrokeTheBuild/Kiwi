#ifndef KIWI_UTILITY_Exception_HPP
#define KIWI_UTILITY_Exception_HPP

#include <Kiwi/Utility/Config.hpp>
#include <Kiwi/Utility/INonCopyable.hpp>
#include <Kiwi/Utility/String.hpp>
#include <Kiwi/Utility/Log.hpp>

#include <stdexcept>
#include <utility>

namespace kiwi {

class KIWI_UTILITY_API RuntimeError : public std::runtime_error, INonCopyable
{
public:
    
    RuntimeError(String message)
        : std::runtime_error(message)
    { }

    template <class... Args>
    RuntimeError(StringView format, Args&&... args)
        : std::runtime_error(fmt::vformat(format, fmt::make_format_args(args...)))
    { }

}; // RuntimeError

} // namespace kiwi

#endif // KIWI_UTILITY_Exception_HPP