
SET(_target KiwiNES)

DEFINE_EMULATOR(${_target} "NES")

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        KiwiMOS6502
)