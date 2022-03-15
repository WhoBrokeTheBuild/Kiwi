# GetRuntimeDir.cmake
#
# Finds the runtime output directory for a given ${_target} and stores it in ${_out_var}
#
# This will search IMPORTED_LOCATION and IMPORTED_LOCATION_${CMAKE_BUILD_TYPE} in that order.
#
# This directory will need to be added to the PATH/LD_LIBRARY_PATH in order for executables
# that link against this target to run.
#

MACRO(GET_RUNTIME_DIR _target _out_var)
    IF(TARGET ${_target})
        GET_TARGET_PROPERTY(_location ${_target} IMPORTED_LOCATION)
        GET_FILENAME_COMPONENT(${_out_var} ${_location} DIRECTORY)

        IF (${_out_var} STREQUAL "" AND CMAKE_BUILD_TYPE)
            STRING(TOUPPER ${CMAKE_BUILD_TYPE} _type)
            GET_TARGET_PROPERTY(_location ${_target} IMPORTED_LOCATION_${_type})
            GET_FILENAME_COMPONENT(${_out_var} ${_location} DIRECTORY)
        ENDIF()
    ELSE()
        MESSAGE(WARNING "GET_RUNTIME_DIR() Unable to find target '${_target}'")
    ENDIF()
ENDMACRO()