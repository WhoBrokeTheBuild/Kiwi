#ifndef KIWI_MOS6502_HPP
#define KIWI_MOS6502_HPP

#include <cstdint>

namespace kiwi {

namespace MOS6502 {

class Processor
{
public:

    // Program Counter
    uint16_t PC;

    // Stack Pointer
    uint8_t SP;

    // Accumulator
    uint8_t A;

    // X Register
    uint8_t X;

    // Y Register
    uint8_t Y;

    // Status Register
    union {
        struct {
            // Carry
            uint8_t C : 1;

            // Zero
            uint8_t Z : 1;

            // Interrupt Disable
            uint8_t I : 1;

            // Decimal
            uint8_t D : 1;
            
            // B
            uint8_t B : 1;

            uint8_t   : 1;

            // Overflow
            uint8_t V : 1;

            // Negative
            uint8_t N : 1;
        };

        uint8_t SR;
    };
    

private:



}; // class CPU

} // namespace MOS6502
    
} // namespace kiwi

#endif // KIWI_MOS6502_HPP