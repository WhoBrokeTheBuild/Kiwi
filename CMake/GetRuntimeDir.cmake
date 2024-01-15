# GetRuntimeDir.cmake
#
# Finds the runtime output directory for a given ${_target} and stores it in ${_out_var}
#
# This will search IMPORTED_LOCATION and IMPORTED_LOCATION_${CMAKE_BUILD_TYPE} in that order.
#
# This directory will need to be added to the PATH/LD_LIBRARY_PATH in order for executables
# that link against this target to run.
#

macro(GET_RUNTIME_DIR _target _out_var)
    if(TARGET ${_target})
        get_target_property(_location ${_target} IMPORTED_LOCATION)
        get_filename_component(${_out_var} ${_location} DIRECTORY)

        if(${_out_var} STREQUAL "" AND CMAKE_BUILD_TYPE)
            string(TOUPPER ${CMAKE_BUILD_TYPE} _type)
            get_target_property(_location ${_target} IMPORTED_LOCATION_${_type})
            get_filename_component(${_out_var} ${_location} DIRECTORY)
        endif()
    else()
        message(WARNING "GET_RUNTIME_DIR() Unable to find target '${_target}'")
    endif()
endmacro()