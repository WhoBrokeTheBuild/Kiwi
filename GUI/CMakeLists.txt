
SET(_target GUI)

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

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        OUTPUT_NAME "Kiwi"
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

###
### Launch Targets
###

ADD_LAUNCH_TARGET(
    Kiwi
    ${CMAKE_CURRENT_SOURCE_DIR}
    $<TARGET_FILE:${_target}>
)

FOREACH(_emulator_target ${KIWI_EMULATOR_TARGET_LIST})

    ADD_LAUNCH_TARGET(
        "Kiwi ${_emulator_target}"
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<TARGET_FILE:${_target}>
        --emulator ${_emulator_target}
    )

ENDFOREACH()