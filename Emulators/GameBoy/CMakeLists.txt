
SET(_target KiwiGameBoy)

DEFINE_EMULATOR(${_target} "KIWI_GAMEBOY")

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        KiwiSM83
)