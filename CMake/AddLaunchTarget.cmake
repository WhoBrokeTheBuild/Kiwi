# AddLaunchTarget.cmake
#
# This will run add a launch target for Visual Studio Code, and Visual Studio (if on windows).
# TODO:
#

MACRO(ADD_LAUNCH_TARGET _name _target _working_dir)

    IF(WIN32)
        SET(_ext ".exe")
    ENDIF()

    SET(_executable ${CMAKE_CURRENT_BINARY_DIR}/${_target}${_ext})
    FILE(RELATIVE_PATH _executable ${CMAKE_BINARY_DIR} ${_executable})

    EXECUTE_PROCESS(
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/Scripts/add-launch-target.py
            --name "${_name}"
            --binary-dir "${CMAKE_BINARY_DIR}"
            --working-dir "${_working_dir}"
            --runtime-path "${KIWI_RUNTIME_PATH}"
            --executable "${_executable}"
            -- ${ARGN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

ENDMACRO()