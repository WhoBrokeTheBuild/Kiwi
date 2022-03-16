# AddLaunchTarget.cmake
#
# This will run add a launch target for Visual Studio Code, and Visual Studio (if on windows).
# TODO:
#

ADD_CUSTOM_TARGET(
    launch-targets ALL
)

MACRO(ADD_LAUNCH_TARGET _id _name _target)

    SET(_runtime_path "${KIWI_RUNTIME_PATH}")

    IF(NOT WIN32)
        STRING(REPLACE ";" ":" _runtime_path "${_runtime_path}")
    ENDIF()

    ADD_CUSTOM_TARGET(
        launch-target-${_id}
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/Scripts/add-launch-target.py
            --name "${_name}"
            --binaryDir "${CMAKE_BINARY_DIR}"
            --runtimePath "${_runtime_path}"
            --executable $<TARGET_FILE:${_target}>
            -- ${ARGN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    ADD_DEPENDENCIES(
        launch-targets
        launch-target-${_id}
    )

    # Manually force a dependency chain so they can't run in parallel
    IF(_previous_launch_target)
        ADD_DEPENDENCIES(
            ${_previous_launch_target}
            launch-target-${_id}
        )
    ENDIF()

    SET(_previous_launch_target launch-target-${_id})

ENDMACRO()