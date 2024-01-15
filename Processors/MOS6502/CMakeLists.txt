
###
### MOS 6502
###

kiwi_define_processor(MOS6502)

target_compile_definitions(
    MOS6502
    PUBLIC
        MODEL=MOS6502
)

###
### MOS 6507
###

kiwi_define_processor(MOS6507)

target_compile_definitions(
    MOS6507
    PUBLIC
        MODEL=MOS6507
)

###
### MOS 6510
###

set(_target MOS6510)

kiwi_define_processor(MOS6510)

target_compile_definitions(
    MOS6510
    PUBLIC
        MODEL=MOS6510
)