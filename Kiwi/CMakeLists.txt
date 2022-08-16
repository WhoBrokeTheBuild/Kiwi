
SET(_library_target Kiwi)

###
### Shader Processing
###

SET(KIWI_SHADER_INCLUDE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    ${KIWI_SHADER_INCLUDE_PATH}
    CACHE INTERNAL ""
)

FILE(
    GLOB_RECURSE
    _shader_input_list
    "Assets/*.glsl"
)

INCLUDE(CompileShaderList)

COMPILE_SHADER_LIST("${KIWI_SHADER_INCLUDE_PATH}" "${_shader_input_list}" _shader_output_list)

INCLUDE(GenerateAssetIncludeList)

GENERATE_ASSET_INCLUDE_LIST("KIWI" "${_shader_output_list}" _shader_output_include_list)

###
### Target Configuration
###

ADD_LIBRARY(${_library_target} SHARED "")

CONFIGURE_LIBRARY(${_library_target})

TARGET_SOURCES(
    ${_library_target}
    PRIVATE
        ${_shader_output_include_list}
)

TARGET_INCLUDE_DIRECTORIES(
    ${_library_target}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/Assets
)

TARGET_LINK_LIBRARIES(
    ${_library_target}
    PUBLIC
        KiwiUtility

        Qt6::Widgets
        SDL2::SDL2
        ${CMAKE_DL_LIBS}
)

SET_TARGET_PROPERTIES(
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

SET(_executable_target KiwiGUI)

FILE(
    GLOB_RECURSE
    _source_list
    "Source/*.h"
    "Source/*.hpp"
    "Source/*.c"
    "Source/*.cpp"
)

ADD_EXECUTABLE(
    ${_executable_target}
    ${_source_list}
)

SET_TARGET_PROPERTIES(
    ${_executable_target}
    PROPERTIES
        OUTPUT_NAME "Kiwi"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

TARGET_LINK_LIBRARIES(
    ${_executable_target}
    PUBLIC
        Kiwi
)
