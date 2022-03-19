
SET(_target GameBoy)

DEFINE_EMULATOR(${_target})

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        SM83
)