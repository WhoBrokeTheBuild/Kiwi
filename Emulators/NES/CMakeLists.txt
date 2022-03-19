
SET(_target NES)

DEFINE_EMULATOR(${_target})

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        MOS6502
)