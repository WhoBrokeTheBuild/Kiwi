#ifndef KIWI_SM83_HPP
#define KIWI_SM83_HPP

#include <cstdint>

namespace kiwi {
namespace SM83 {

class Processor
{
public:

    ///
    /// Registers
    ///

    union {
        struct {
            union {
                struct {
                    uint8_t : 4;

                    // Zero Flag
                    bool FZ : 1;

                    // Subtract Flag
                    bool FN : 1;

                    // Half-Carry Flag
                    bool FH : 1;

                    // Carry Flag
                    bool FC : 1;
                };
                
                // Raw Flags Register
                uint8_t F;
            };

            // Accumulator Register
            uint8_t A;
        };

        // (A << 8) | F
        uint16_t AF;
    };

    union {
        struct {
            // General Purpose Register
            uint8_t C;

            // General Purpose Register
            uint8_t B;
        };

        // (B << 8) | C
        uint16_t BC;
    };

    union {
        struct {
            // General Purpose Register
            uint8_t E;
            
            // General Purpose Register
            uint8_t D;
        };

        // (D << 8) | E
        uint16_t DE;
    };

    union {
        struct {
            // Low Address Register
            uint8_t L;

            // High Address Register
            uint8_t H;
        };
        
        // (H << 8) | L
        uint16_t HL;
    };

    // Program Counter
    uint16_t PC;

    // Stack Pointer
    uint16_t SP;

    

    Processor();
    
    virtual ~Processor();

private:


}; // class Processor

} // namespace SM83
} // namespace kiwi

#endif // KIWI_SM83_HPP