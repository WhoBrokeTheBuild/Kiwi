
MACRO(DEFINE_CHIP _target)
        
    ADD_LIBRARY(${_target} STATIC "")

    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            OUTPUT_NAME "Kiwi${_target}"
    )
    
    CONFIGURE_LIBRARY(${_target})

    TARGET_LINK_LIBRARIES(
        ${_target}
        PUBLIC
            Kiwi
    )

ENDMACRO()

###
### Chip Libraries
###

ADD_SUBDIRECTORY(DMG-CPU)
# ADD_SUBDIRECTORY(CGB-CPU)

ADD_SUBDIRECTORY(MOS6502)
# ADD_SUBDIRECTORY(RP2A03)
