# ConfigureFileList.cmake
#
# This will run CONFIGURE_FILE for each file in ${_input_list}, and store them in their 
# relative location under the binary directory, with the last extension stripped.
# The resulting list of filenames will be stored in ${_output_list}.
#
# For example:
#   ${CMAKE_CURRENT_SOURCE_DIR}/Source/Config.hpp.in
# will be configured and stored in 
#   ${CMAKE_CURRENT_BINARY_DIR}/Source/Config.hpp
#

MACRO(CONFIGURE_FILE_LIST _input_list _output_list_variable)
    FOREACH(_input ${_input_list})
        # Replace leading source directory in path with binary directory
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            _output
            ${_input}
        )

        # Remove the ".in"
        STRING(REGEX
            MATCH "^(.*)\\.[^.]*$"
            _output
            ${_output}
        )
        SET(_output ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${_input} ${_output})

        LIST(APPEND ${_output_list_variable} ${_output})
    ENDFOREACH()
ENDMACRO()