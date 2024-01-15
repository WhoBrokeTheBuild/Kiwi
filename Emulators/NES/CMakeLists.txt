
set(_target NES)

kiwi_define_emulator(${_target})

target_link_libraries(
    ${_target}
    PUBLIC
        MOS6502
)