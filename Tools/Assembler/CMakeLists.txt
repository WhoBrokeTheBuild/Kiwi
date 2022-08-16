
SET(_target KiwiASM)

FILE(
    GLOB_RECURSE
    _source_list
    "Source/*.h"
    "Source/*.c"
    "Source/*.hpp"
    "Source/*.cpp"
)

ADD_EXECUTABLE(
    ${_target}
    ${_source_list}
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PRIVATE
        Kiwi
        cflags::cppflags
)

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)