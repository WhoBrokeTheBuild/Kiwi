
SET(_target KiwiCommon)

ADD_LIBRARY(${_target} STATIC "")

CONFIGURE_LIBRARY(${_target})

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        fmt::fmt
)
