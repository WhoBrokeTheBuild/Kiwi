
set(_target KiwiASM)

file(
    GLOB_RECURSE
    _source_list
    "Source/*.h"
    "Source/*.c"
    "Source/*.hpp"
    "Source/*.cpp"
)

add_executable(
    ${_target}
    ${_source_list}
)

target_link_libraries(
    ${_target}
    PRIVATE
        Kiwi
        cflags::cppflags
)
