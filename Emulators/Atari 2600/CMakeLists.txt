
set(_target Atari2600)

kiwi_define_emulator(${_target})

target_link_libraries(
    ${_target}
    PUBLIC
        MOS6507
)