
set(_target GameBoy)

kiwi_define_emulator(${_target})

target_link_libraries(
    ${_target}
    PUBLIC
        DMG-CPU
)