
set(_library_target Kiwi)

###
### Shader Processing
###

set(KIWI_SHADER_INCLUDE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    ${KIWI_SHADER_INCLUDE_PATH}
    CACHE INTERNAL ""
)

file(
    GLOB_RECURSE
    _shader_input_list
    "Assets/*.glsl"
)

include(KiwiCompileShaderList)

kiwi_compile_shader_list("${KIWI_SHADER_INCLUDE_PATH}" "${_shader_input_list}" _shader_output_list)

include(KiwiGenerateAssetIncludeList)

kiwi_generate_asset_include_list("KIWI" "${_shader_output_list}" _shader_output_include_list)

###
### Target Configuration
###

add_library(${_library_target} SHARED "")

kiwi_configure_library(${_library_target})

target_sources(
    ${_library_target}
    PRIVATE
        ${_shader_output_include_list}
)

target_include_directories(
    ${_library_target}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Public/Kiwi/Widgets
        ${CMAKE_CURRENT_BINARY_DIR}/Assets
)

target_link_libraries(
    ${_library_target}
    PUBLIC
        KiwiUtility

        Qt6::Widgets
        SDL2::SDL2
        ${CMAKE_DL_LIBS}
)

set_target_properties(
    ${_library_target}
    PROPERTIES
        DEFINE_SYMBOL "KIWI_EXPORT"

        AUTOMOC ON # Qt Meta-Object Compiler
        AUTORCC ON # Qt Resource Compiler
        AUTOUIC ON # Qt UI Compiler
)

###
### Executable Configuration
###

set(_executable_target KiwiGUI)

file(
    GLOB_RECURSE
    _source_list
    "Source/*.h"
    "Source/*.hpp"
    "Source/*.c"
    "Source/*.cpp"
)

add_executable(
    ${_executable_target}
    ${_source_list}
)

set_target_properties(
    ${_executable_target}
    PROPERTIES
        OUTPUT_NAME "Kiwi"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_link_libraries(
    ${_executable_target}
    PUBLIC
        Kiwi
)
