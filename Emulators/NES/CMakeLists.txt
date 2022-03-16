
SET(_target KiwiNES)

DEFINE_EMULATOR(${_target})

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        KiwiMOS6502
)