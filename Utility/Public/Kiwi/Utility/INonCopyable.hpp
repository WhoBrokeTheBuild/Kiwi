#ifndef KIWI_UTILITY_INonCopyable_HPP
#define KIWI_UTILITY_INonCopyable_HPP

namespace kiwi {

///
/// Interface for explicitly disabling the default Copy Constructor and Assignment Operator.
///
class INonCopyable
{
private:

    INonCopyable(const INonCopyable&) = delete;

    INonCopyable& operator=(const INonCopyable) = delete;

protected:

    INonCopyable() = default;

    ~INonCopyable() = default;

}; // class INonCopyable

} // namespace kiwi

#endif // KIWI_UTILITY_INonCopyable_HPP