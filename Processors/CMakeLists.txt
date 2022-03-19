
MACRO(DEFINE_PROCESSOR _target)
        
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
### Processor Libraries
###

ADD_SUBDIRECTORY(MOS6502)
ADD_SUBDIRECTORY(SM83)
