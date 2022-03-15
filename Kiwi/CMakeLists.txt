
SET(_target Kiwi)

###
### Source Files
###

FILE(
    GLOB_RECURSE
    _source_list
    "Public/*.h"
    "Public/*.hpp"
    "Private/*.h"
    "Private/*.hpp"
    "Private/*.c"
    "Private/*.cpp"
)

FILE(
    GLOB_RECURSE
    _source_input_list
    "Public/*.in"
    "Private/*.in"
)

INCLUDE(ConfigureFileList)

CONFIGURE_FILE_LIST("${_source_input_list}" _source_output_list)

ADD_EXECUTABLE(
    ${_target}
    ${_source_list}
    ${_source_output_list}
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        KiwiCommon
        Qt5::Widgets
)

INCLUDE(AddLaunchTarget)

IF(WIN32)
    SET(_ext ".exe")
ENDIF()

SET(_executable ${CMAKE_CURRENT_BINARY_DIR}/${_target}${_ext})

ADD_LAUNCH_TARGET(
    "${_target}"
    ${_target}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

FOREACH(_emulator ${KIWI_EMULATOR_TARGET_LIST})

    ADD_LAUNCH_TARGET(
        "${_target} ${_emulator}"
        ${_target}
        ${CMAKE_CURRENT_SOURCE_DIR}
        --emulator ${_emulator}
    )

ENDFOREACH()