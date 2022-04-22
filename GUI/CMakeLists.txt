
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

        AUTOMOC ON # Qt Meta-Object Compiler
        AUTORCC ON # Qt Resource Compiler
        AUTOUIC ON # Qt UI Compiler
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        Kiwi
)

###
### Launch Targets
###

ADD_LAUNCH_TARGET(
    Kiwi
    ${CMAKE_BINARY_DIR}
    $<TARGET_FILE:${_target}>
)

FOREACH(_emulator_target ${KIWI_EMULATOR_TARGET_LIST})

    ADD_LAUNCH_TARGET(
        "Kiwi ${_emulator_target}"
        ${CMAKE_BINARY_DIR}
        $<TARGET_FILE:${_target}>
        --emulator $<TARGET_FILE:${_emulator_target}>
    )

ENDFOREACH()