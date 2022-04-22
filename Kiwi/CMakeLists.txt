
SET(_target Kiwi)

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

ADD_LIBRARY(${_target} SHARED "")

CONFIGURE_LIBRARY(${_target})

TARGET_SOURCES(
    ${_target}
    PRIVATE
        ${_shader_output_include_list}
)

TARGET_INCLUDE_DIRECTORIES(
    ${_target}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/Assets
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        fmt::fmt
        nlohmann_json::nlohmann_json
        cflags::cppflags
        Qt6::Widgets
)

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        DEFINE_SYMBOL "KIWI_EXPORT"

        AUTOMOC ON # Qt Meta-Object Compiler
        AUTORCC ON # Qt Resource Compiler
        AUTOUIC ON # Qt UI Compiler
)

SET(KIWI_RUNTIME_PATH_LIST
    ${KIWI_RUNTIME_PATH_LIST}
    ${CMAKE_CURRENT_BINARY_DIR}
    CACHE INTERNAL ""
)
