
SET(_target KiwiGameBoy)

DEFINE_EMULATOR(${_target})

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        KiwiSM83
)