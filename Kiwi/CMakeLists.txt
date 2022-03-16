
###
### Library
###

SET(_target Kiwi)

ADD_LIBRARY(${_target} SHARED "")

CONFIGURE_LIBRARY(${_target})

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        fmt::fmt
)

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        DEFINE_SYMBOL "KIWI_EXPORT"
)

###
### Executable
###

SET(_target KiwiGUI)

FILE(
    GLOB_RECURSE
    _source_list
    "Source/*.h"
    "Source/*.hpp"
    "Source/*.c"
    "Source/*.cpp"
)

ADD_EXECUTABLE(
    ${_target}
    ${_source_list}
)

TARGET_INCLUDE_DIRECTORIES(
    ${_target}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
        ${CMAKE_CURRENT_BINARY_DIR}/Source
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        Kiwi
        Qt5::Widgets
)

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        OUTPUT_NAME "Kiwi"
)
