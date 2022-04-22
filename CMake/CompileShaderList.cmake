# CompileShaders.cmake
#
# This will run glslc for each file in ${_input_list}, and store the resulting .spv 
# in their relative location under the binary directory. 
# The ${_include_path} values will be appended to the glslc command, each prefixed with -I.
# The resulting list of filenames will be stored in ${_output_list}.
#
# For example:
#   ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/Default.vert.glsl
# will be compiled and output to
#   ${CMAKE_BINARY_SOURCE_DIR}/Assets/Shaders/Default.vert.spv
#

MACRO(COMPILE_SHADER_LIST _include_path _input_list _output_list)
    FOREACH(_input ${_input_list})

        GET_FILENAME_COMPONENT(_input_ext ${_input} LAST_EXT)

        IF(_input_ext STREQUAL ".glsl")

            # /path/to/Default.vert.glsl -> Default.vert 
            GET_FILENAME_COMPONENT(_input_name ${_input} NAME_WLE)

            # /path/to/Default.vert.glsl -> /path/to/
            GET_FILENAME_COMPONENT(_input_path ${_input} DIRECTORY)

            # Replace leading source directory in path with binary directory
            STRING(REPLACE 
                ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}
                _output_path
                ${_input_path}
            )

            FILE(MAKE_DIRECTORY ${_output_path})

            # Default.vert -> .vert
            GET_FILENAME_COMPONENT(_stage ${_input_name} LAST_EXT)

            # .vert -> vert
            STRING(SUBSTRING ${_stage} 1 -1 _stage)

            IF(_stage STREQUAL "inc")
                CONTINUE()
            ENDIF()
            
            SET(_include_flags "")
            FOREACH(_path ${_include_path})
                SET(_include_flags ${_include_flags} -I"${_path}")
            ENDFOREACH()

            SET(_output "${_output_path}/${_input_name}.spv")

            ADD_CUSTOM_COMMAND(
                OUTPUT ${_output}
                DEPENDS ${_input}
                COMMAND ${Vulkan_GLSLC_EXECUTABLE}
                    -fshader-stage=${_stage}
                    --target-env=vulkan
                    ${_include_flags}
                    -MD -MF ${_output}.d
                    -o ${_output}
                    ${_input}
                DEPFILE ${_output}.d
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
            
            LIST(APPEND ${_output_list} ${_output})
        ENDIF()
    ENDFOREACH()
ENDMACRO()