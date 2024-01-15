
macro(kiwi_define_emulator _target)
    
    add_library(${_target} SHARED "")

    set_target_properties(
        ${_target}
        PROPERTIES
            OUTPUT_NAME "Kiwi${_target}"
    
            AUTOMOC ON # Qt Meta-Object Compiler
            AUTORCC ON # Qt Resource Compiler
            AUTOUIC ON # Qt UI Compiler
    )

    kiwi_configure_library(${_target})

    target_link_libraries(
        ${_target}
        PUBLIC
            Kiwi
    )

    set(KIWI_EMULATOR_TARGET_LIST
        ${KIWI_EMULATOR_TARGET_LIST}
        ${_target}
        CACHE INTERNAL ""
    )

endmacro()

###
### Emulator Libraries & Targets
###

unset(KIWI_EMULATOR_TARGET_LIST CACHE)

# add_subdirectory(CHIP-8)
add_subdirectory(GameBoy)
# add_subdirectory(NES)
