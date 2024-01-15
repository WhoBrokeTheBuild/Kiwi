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

macro(kiwi_generate_asset_include_list _guard_prefix _input_list _output_list_variable)
    foreach(_input ${_input_list})
        # Replace leading source directory in path with binary directory
        string(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            _output
            ${_input}
        )

        set(_output "${_input}.hpp")

        cmake_path(GET _input FILENAME _variable)
        string(REPLACE "." "_" _variable "${_variable}")

        string(TOUPPER "${_variable}" _guard)
        set(_guard "${_guard_prefix}_${_guard}_HPP")

        add_custom_command(
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

        list(APPEND ${_output_list_variable} ${_output})
    endforeach()
endmacro()