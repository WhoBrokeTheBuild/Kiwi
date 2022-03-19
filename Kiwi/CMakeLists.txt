
SET(_target Kiwi)

ADD_LIBRARY(${_target} SHARED "")

CONFIGURE_LIBRARY(${_target})

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        fmt::fmt
)

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        DEFINE_SYMBOL "KIWI_EXPORT"
)

SET(KIWI_LAUNCH_CONFIG_LIST ${KIWI_LAUNCH_CONFIG_LIST} PARENT_SCOPE)