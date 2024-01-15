
macro(kiwi_define_processor _target)
        
    add_library(${_target} STATIC "")

    set_target_properties(
        ${_target}
        PROPERTIES
            OUTPUT_NAME "Kiwi${_target}"
    )
    
    kiwi_configure_library(${_target})

    target_link_libraries(
        ${_target}
        PUBLIC
            Kiwi
    )

endmacro()

###
### Processor Libraries
###

add_subdirectory(DMG-CPU)
# add_subdirectory(CGB-CPU)
add_subdirectory(MOS6502)
# add_subdirectory(RP2A03)
