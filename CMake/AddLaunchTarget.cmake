# AddLaunchTarget.cmake
#
# This will add a launch target for Visual Studio Code, and Visual Studio (if on Windows).
# Add launch targets with ADD_LAUNCH_TARGET(). After all targets have been added, call 
# GENERATE_LAUNCH_TARGET_CONFIG() from the top level CMakeLists.txt.
#

IF(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    MESSAGE(FATAL_ERROR "AddLaunchTarget.cmake must be included at the top of your project")
ENDIF()

UNSET(KIWI_LAUNCH_TARGET_NAME_LIST CACHE)
UNSET(KIWI_LAUNCH_TARGET_WORKING_DIR_LIST CACHE)
UNSET(KIWI_LAUNCH_TARGET_EXECUTABLE_LIST CACHE)
UNSET(KIWI_LAUNCH_TARGET_ARGUMENTS_LIST CACHE)

MACRO(ADD_LAUNCH_TARGET _name _workingDir _executable)

    SET(KIWI_LAUNCH_TARGET_NAME_LIST
        ${KIWI_LAUNCH_TARGET_NAME_LIST}
        "${_name}"
        CACHE INTERNAL ""
    )

    SET(KIWI_LAUNCH_TARGET_WORKING_DIR_LIST
        ${KIWI_LAUNCH_TARGET_WORKING_DIR_LIST}
        "${_workingDir}"
        CACHE INTERNAL ""
    )

    SET(KIWI_LAUNCH_TARGET_EXECUTABLE_LIST
        ${KIWI_LAUNCH_TARGET_EXECUTABLE_LIST}
        "${_executable}"
        CACHE INTERNAL ""
    )

    SET(_arguments "${ARGN}")
    LIST(JOIN _arguments " " _arguments)

    SET(KIWI_LAUNCH_TARGET_ARGUMENTS_LIST
        ${KIWI_LAUNCH_TARGET_ARGUMENTS_LIST}
        "-- ${_arguments}"
        CACHE INTERNAL ""
    )

ENDMACRO()

FUNCTION(GENERATE_LAUNCH_TARGET_CONFIG)

    SET(_runtime_path_list "${KIWI_RUNTIME_PATH_LIST}")

    IF(NOT WIN32)
        LIST(JOIN "${_runtime_path_list}" ":" _runtime_path_list)
    ENDIF()

    ADD_CUSTOM_TARGET(launch-target-config ALL)

    SET(_previous_target launch-target-config)

    FOREACH(_name _workingDir _executable _arguments 
        IN ZIP_LISTS 
            KIWI_LAUNCH_TARGET_NAME_LIST
            KIWI_LAUNCH_TARGET_WORKING_DIRECTORY_LIST
            KIWI_LAUNCH_TARGET_EXECUTABLE_LIST
            KIWI_LAUNCH_TARGET_ARGUMENTS_LIST)

        STRING(MAKE_C_IDENTIFIER "${_name}" _id)

        # Force CMake to not surround ${_arguments} in quotes
        SEPARATE_ARGUMENTS(_argumentList NATIVE_COMMAND ${_arguments})

        ADD_CUSTOM_TARGET(
            launch-target-config-${_id}
            COMMENT "Adding Launch Target for '${_name}'"
            COMMAND ${Python3_EXECUTABLE}
                ${CMAKE_SOURCE_DIR}/Scripts/add-launch-target.py
                --name "${_name}"
                --executable "${_executable}" 
                --runtimePathList "${_runtime_path_list}"
                --binaryDir "${CMAKE_BINARY_DIR}"
                --workingDir "${_workingDir}"
                ${_argumentList}
            VERBATIM
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )

        ADD_DEPENDENCIES(${_previous_target} launch-target-config-${_id})
        SET(_previous_target launch-target-config-${_id})

    ENDFOREACH()

ENDFUNCTION()
