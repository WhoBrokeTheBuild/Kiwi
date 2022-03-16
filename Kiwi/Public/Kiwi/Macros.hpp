#ifndef KIWI_MACROS_HPP
#define KIWI_MACROS_HPP

#if defined(_MSC_VER)

    #define KIWI_PACK(DECL)                                                   \
        __pragma(pack(push, 1))                                               \
        DECL                                                                  \
        __pragma(pack(pop))

#elif defined(__GNUC__)

    #define KIWI_PACK(DECL)                                                   \
        DECL __attribute__((__packed__))

#endif

#define KIWI_UNUSED(V) (void)(V)

#endif // KIWI_MACROS_HPP