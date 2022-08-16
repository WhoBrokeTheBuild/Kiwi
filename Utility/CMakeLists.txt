
SET(_target KiwiUtility)

###
### Target Configuration
###

ADD_LIBRARY(${_target} SHARED "")

CONFIGURE_LIBRARY(${_target})

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        fmt::fmt
        nlohmann_json::nlohmann_json
        cflags::cppflags
)

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        DEFINE_SYMBOL "KIWI_UTILITY_EXPORT"
)
