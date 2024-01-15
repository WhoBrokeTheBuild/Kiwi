
SET(_target KiwiUtility)

###
### Target Configuration
###

add_library(${_target} SHARED "")

kiwi_configure_library(${_target})

target_link_libraries(
    ${_target}
    PUBLIC
        glm::glm
        fmt::fmt
        nlohmann_json::nlohmann_json
        cflags::cppflags
)

set_target_properties(
    ${_target}
    PROPERTIES
        DEFINE_SYMBOL "KIWI_UTILITY_EXPORT"
)
