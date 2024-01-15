# AddLaunchTarget.cmake
#
# This will add a launch target for Visual Studio Code, and Visual Studio (if on Windows).
# Add launch targets with ADD_LAUNCH_TARGET(). After all targets have been added, call 
# GENERATE_LAUNCH_TARGET_CONFIG() from the top level CMakeLists.txt.
#

if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "AddLaunchTarget.cmake must be included at the top of your project")
endif()

unset(KIWI_LAUNCH_TARGET_NAME_LIST CACHE)
unset(KIWI_LAUNCH_TARGET_WORKING_DIRECTORY_LIST CACHE)
unset(KIWI_LAUNCH_TARGET_EXECUTABLE_LIST CACHE)
unset(KIWI_LAUNCH_TARGET_ARGUMENTS_LIST CACHE)

macro(kiwi_add_launch_target _name _workingDir _executable)

    set(KIWI_LAUNCH_TARGET_NAME_LIST
        ${KIWI_LAUNCH_TARGET_NAME_LIST}
        "${_name}"
        CACHE INTERNAL ""
    )

    set(KIWI_LAUNCH_TARGET_WORKING_DIRECTORY_LIST
        ${KIWI_LAUNCH_TARGET_WORKING_DIRECTORY_LIST}
        "${_workingDir}"
        CACHE INTERNAL ""
    )

    set(KIWI_LAUNCH_TARGET_EXECUTABLE_LIST
        ${KIWI_LAUNCH_TARGET_EXECUTABLE_LIST}
        "${_executable}"
        CACHE INTERNAL ""
    )

    set(_arguments "${ARGN}")
    list(JOIN _arguments " " _arguments)

    set(KIWI_LAUNCH_TARGET_ARGUMENTS_LIST
        ${KIWI_LAUNCH_TARGET_ARGUMENTS_LIST}
        "-- ${_arguments}"
        CACHE INTERNAL ""
    )

endmacro()

function(kiwi_generate_launch_target_config)

    set(_runtime_path_list "${KIWI_RUNTIME_PATH_LIST}")

    if(NOT WIN32)
        list(JOIN "${_runtime_path_list}" ":" _runtime_path_list)
    endif()

    add_custom_target(launch-target-config ALL)

    set(_previous_target launch-target-config)

    foreach(_name _workingDir _executable _arguments 
        IN ZIP_LISTS 
            KIWI_LAUNCH_TARGET_NAME_LIST
            KIWI_LAUNCH_TARGET_WORKING_DIRECTORY_LIST
            KIWI_LAUNCH_TARGET_EXECUTABLE_LIST
            KIWI_LAUNCH_TARGET_ARGUMENTS_LIST)

        string(MAKE_C_IDENTIFIER "${_name}" _id)

        # Force CMake to not surround ${_arguments} in quotes
        separate_arguments(_argumentList NATIVE_COMMAND ${_arguments})

        add_custom_target(
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

        add_dependencies(${_previous_target} launch-target-config-${_id})
        set(_previous_target launch-target-config-${_id})

    endforeach()

endfunction()
