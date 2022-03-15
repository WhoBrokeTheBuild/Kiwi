#ifndef KIWI_MOS6502_HPP
#define KIWI_MOS6502_HPP

#include <cstdint>

namespace kiwi {

namespace MOS6502 {

struct Registers
{
    union {
        struct {
            // Carry Flag
            uint8_t C : 1;

            // Zero Flag
            uint8_t Z : 1;

            // Interrupt Flag
            uint8_t I : 1;

            // Decimal Flag
            uint8_t D : 1;

            // Break Flag
            uint8_t B : 1;

            // Unused Flag
            uint8_t   : 1;

            // Overflow Flag
            uint8_t V : 1;

            // Negative Flag
            uint8_t N : 1;

        };

        uint8_t F;
    };
};

class CPU
{
public:

    

private:



}; // class CPU

} // namespace MOS6502
    
} // namespace kiwi

#endif // KIWI_MOS6502_HPP