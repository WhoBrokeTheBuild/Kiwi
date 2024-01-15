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

macro(kiwi_compile_shader_list _include_path _input_list _output_list)
    foreach(_input ${_input_list})

        get_filename_component(_input_ext ${_input} LAST_EXT)

        if(_input_ext STREQUAL ".glsl")

            # /path/to/Default.vert.glsl -> Default.vert 
            get_filename_component(_input_name ${_input} NAME_WLE)

            # /path/to/Default.vert.glsl -> /path/to/
            get_filename_component(_input_path ${_input} DIRECTORY)

            # Replace leading source directory in path with binary directory
            string(REPLACE 
                ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}
                _output_path
                ${_input_path}
            )

            file(MAKE_DIRECTORY ${_output_path})

            # Default.vert -> .vert
            get_filename_component(_stage ${_input_name} LAST_EXT)

            # .vert -> vert
            string(SUBSTRING ${_stage} 1 -1 _stage)

            if(_stage STREQUAL "inc")
                continue()
            endif()
            
            set(_include_flags "")
            foreach(_path ${_include_path})
                set(_include_flags ${_include_flags} -I"${_path}")
            endforeach()

            set(_output "${_output_path}/${_input_name}.spv")

            add_custom_command(
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
            
            list(APPEND ${_output_list} ${_output})
        endif()
    endforeach()
endmacro()