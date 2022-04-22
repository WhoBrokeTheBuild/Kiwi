# GenerateAssetIncludeList.cmake
#
# This will call ADD_CUSTOM_COMMAND for each file in ${_input_list}, which will call 
# generate-asset-include.py. This will generate a file with the name ${_input}.hpp, and 
# store it in the relative location under the binary directory.
# The variable name will be ${_input}, with .'s replaced by _'s.
# The #ifndef guards will be ${_guard_prefix}_${TOUPPER(_variable)}_HPP 
#
# For example:
#   ${CMAKE_CURRENT_SOURCE_DIR}/Assets/image.png
# will be converted to a unsigned char array and stored in 
#   ${CMAKE_CURRENT_BINARY_DIR}/Assets/image.png.hpp
# With content that looks like the following
#   #ifndef PREFIX_IMAGE_PNG_HPP
#   #define PREFIX_IMAGE_PNG_HPP
#  
#   unsigned char image_png[] = {
#       0x01, 0x02, 0x03, 0x04, ...
#   };
#  
#   #endif // PREFIX_IMAGE_PNG_HPP
#

MACRO(GENERATE_ASSET_INCLUDE_LIST _guard_prefix _input_list _output_list_variable)
    FOREACH(_input ${_input_list})
        # Replace leading source directory in path with binary directory
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            _output
            ${_input}
        )

        SET(_output "${_input}.hpp")

        CMAKE_PATH(GET _input FILENAME _variable)
        STRING(REPLACE "." "_" _variable "${_variable}")

        STRING(TOUPPER "${_variable}" _guard)
        SET(_guard "${_guard_prefix}_${_guard}_HPP")

        ADD_CUSTOM_COMMAND(
            MAIN_DEPENDENCY ${_input}
            OUTPUT ${_output}
            COMMAND ${Python3_EXECUTABLE}
                ${CMAKE_SOURCE_DIR}/Scripts/generate-asset-include.py
                --input "${_input}"
                --output "${_output}"
                --variable "${_variable}"
                --guard "${_guard}"
            VERBATIM
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )

        LIST(APPEND ${_output_list_variable} ${_output})
    ENDFOREACH()
ENDMACRO()