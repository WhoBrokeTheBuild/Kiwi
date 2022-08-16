#include <Kiwi/DMG-CPU/Processor.hpp>

namespace kiwi {
namespace DMG_CPU {

void Processor::checkInterrupts()
{
    if (!IME) {
        return;
    }

    auto callInterrupt = [this](uint16_t address) {
        CPUEnabled = true;
        IME = false;
        pushWord(PC);
        PC = address;
        // _cycles += 5;
    };

    // Requested and Enabled interrupts
    InterruptFlags activeInterrupts = {
        .raw = (uint8_t)(IF.raw & IE.raw)
    };

    if (activeInterrupts.Int40) {
        IF.Int40 = false;
        callInterrupt(0x0040);
        return;
    }

    if (activeInterrupts.Int48) {
        IF.Int48 = false;
        callInterrupt(0x0048);
        return;
    }

    if (activeInterrupts.Int50) {
        IF.Int50 = false;
        callInterrupt(0x0050);
        return;
    }

    if (activeInterrupts.Int58) {
        IF.Int58 = false;
        callInterrupt(0x0058);
        return;
    }

    if (activeInterrupts.Int60) {
        IF.Int60 = false;
        callInterrupt(0x0060);
        return;
    }
}

void Processor::tickCPU()
{
    // if (!CPUEnabled) {
    //     _cycles += 4;
    //     return;
    // }

    // // LastInstructionAddress = R.PC;

    // bool willEnableIME = _requestEnableIME;

    // uint8_t u8;
    // uint16_t u16;
    // uint32_t u32;

    // uint8_t opcode = ReadByte(R.PC);
    // u8 = ReadByte(R.PC + 1);
    // u16 = (ReadByte(R.PC + 2) << 8) | u8;

    // InstructionLog.push_back(fmt::format(
    //     "{:010} AF={:04X} BC={:04X} DE={:04X} HL={:04X} SP={:04X} PC={:04X} {}",
    //     _cycles, R.AF, R.BC, R.DE, R.HL, R.SP, R.PC,
    //     Disassemble(opcode, u8, u16)
    // ));

    // ++R.PC;
    // _cycles += 1;

    // switch (opcode) {

    // // NOP
    // case 0x00:
    //     break;

    // // STOP
    // case 0x10:
    //     // Skip $00
    //     ++R.PC;
    //     // TODO: Improve
    //     break;

    // // HALT
    // case 0x76:
    //     CPUEnabled = false;
    //     // TODO: Improve
    //     break;

    // // DI
    // case 0xF3:
    //     IME = false;
    //     break;

    // // EI
    // case 0xFB:
    //     // Enabling IME is delayed by one instruction
    //     _requestEnableIME = true;
    //     break;

    // // DAA
    // case 0x27:
    //     if (R.FN) {
    //         // Subtraction
    //         if (R.FC) {
    //             R.A -= 0x60;
    //         }
    //         if (R.FH) {
    //             R.A -= 0x06;
    //         }
    //     }
    //     else {
    //         // Addition
    //         if (R.FC || R.A > 0x99) {
    //             R.A += 0x60;
    //             R.FC = true;
    //         }
    //         if (R.FH || (R.A & 0x0F) > 0x09) {
    //             R.A += 0x06;
    //         }
    //     }

    //     R.FZ = (R.A == 0);
    //     R.FH = false;
    //     break;

    // // SFC
    // case 0x37:
    //     R.FN = false;
    //     R.FH = false;
    //     R.FC = true;
    //     break;

    // // CCF
    // case 0x3F:
    //     R.FN = false;
    //     R.FH = false;
    //     R.FC = !R.FC;
    //     break;

    // // CPL
    // case 0x2F:
    //     R.A = ~R.A;
    //     R.FN = true;
    //     R.FH = true;
    //     break;
    
    // #define _CALL(COND)                                                         \
    //     R.PC += 2;                                                              \
    //     _cycles += 3;                                                           \
    //     if (COND) {                                                             \
    //         PushWord(R.PC);                                                     \
    //         _cycles += 2;                                                       \
    //         R.PC = u16;                                                         \
    //     }                                                                       \

    // // CALL u16
    // case 0xCD:
    //     _CALL(true);
    //     break;

    // // CALL NZ,u16
    // case 0xC4:
    //     _CALL(!R.FZ);
    //     break;

    // // CALL Z,u16
    // case 0xCC:
    //     _CALL(R.FZ);
    //     break;

    // // CALL NC,u16
    // case 0xD4:
    //     _CALL(!R.FC);
    //     break;

    // // CALL C,u16
    // case 0xDC:
    //     _CALL(R.FC);
    //     break;

    // #define _RST(ADDR)                                                          \
    //     PushWord(R.PC);                                                         \
    //     _cycles += 3;                                                           \
    //     R.PC = (ADDR)

    // // RST $00
    // case 0xC7:
    //     _RST(0x00);
    //     break;
    
    // // RST $08
    // case 0xCF:
    //     _RST(0x08);
    //     break;
    
    // // RST $10
    // case 0xD7:
    //     _RST(0x10);
    //     break;
    
    // // RST $18
    // case 0xDF:
    //     _RST(0x18);
    //     break;
    
    // // RST $20
    // case 0xE7:
    //     _RST(0x20);
    //     break;
    
    // // RST $28
    // case 0xEF:
    //     _RST(0x28);
    //     break;
    
    // // RST $30
    // case 0xF7:
    //     _RST(0x30);
    //     break;
    
    // // RST $38
    // case 0xFF:
    //     _RST(0x38);
    //     break;

    // // RET
    // case 0xC9:
    //     u16 = PopWord();
    //     _cycles += 3;
    //     R.PC = u16;
    //     break;

    // // RETI
    // case 0xD9:
    //     u16 = PopWord();
    //     _cycles += 3;
    //     R.PC = u16;
    //     IME = true;
    //     break;
    
    // #define _RET(COND)                                                          \
    //     _cycles += 2;                                                           \
    //     if (COND) {                                                             \
    //         u16 = PopWord();                                                    \
    //         _cycles += 3;                                                       \
    //         R.PC = u16;                                                         \
    //     }

    // // RET NZ
    // case 0xC0:
    //     _RET(!R.FZ);
    //     break;
    
    // // RET Z
    // case 0xC8:
    //     _RET(R.FZ);
    //     break;
    
    // // RET NC
    // case 0xD0:
    //     _RET(!R.FC);
    //     break;
    
    // // RET C
    // case 0xD8:
    //     _RET(R.FC);
    //     break;
    
    // // JP (HL)
    // case 0xE9:
    //     R.PC = R.HL;
    //     break;

    // #define _JP(COND)                                                           \
    //     R.PC += 2;                                                              \
    //     _cycles += 2;                                                           \
    //     if (COND) {                                                             \
    //         _cycles += 1;                                                       \
    //         R.PC = u16;                                                         \
    //     }

    // // JP u16
    // case 0xC3:
    //     _JP(true);
    //     break;
    
    // // JP NZ,u16
    // case 0xC2:
    //     _JP(!R.FZ);
    //     break;
    
    // // JP Z,u16
    // case 0xCA:
    //     _JP(R.FZ);
    //     break;
    
    // // JP NC,u16
    // case 0xD2:
    //     _JP(!R.FC);
    //     break;
    
    // // JP C,u16
    // case 0xDA:
    //     _JP(R.FC);
    //     break;
    
    // #define _JR(COND)                                                           \
    //     ++R.PC;                                                                 \
    //     _cycles += 1;                                                           \
    //     if (COND) {                                                             \
    //         _cycles += 1;                                                       \
    //         R.PC += (int8_t)u8;                                                 \
    //     }

    // // Js8
    // case 0x18:
    //     _JR(true);
    //     break;
    
    // // JNZ,s8
    // case 0x20:
    //     _JR(!R.FZ);
    //     break;
    
    // // JZ,s8
    // case 0x28:
    //     _JR(R.FZ);
    //     break;
    
    // // JNC,s8
    // case 0x30:
    //     _JR(!R.FC);
    //     break;
    
    // // JC,s8
    // case 0x38:
    //     _JR(R.FC);
    //     break;

    // // PUSH AF
    // case 0xF5:
    //     PushWord(R.AF);
    //     _cycles += 2;
    //     break;
    
    // // PUSH BC
    // case 0xC5:
    //     PushWord(R.BC);
    //     _cycles += 2;
    //     break;
    
    // // PUSH DE
    // case 0xD5:
    //     PushWord(R.DE);
    //     _cycles += 2;
    //     break;
    
    // // PUSH HL
    // case 0xE5:
    //     PushWord(R.HL);
    //     _cycles += 2;
    //     break;

    // // POP AF
    // case 0xF1:
    //     R.AF = PopWord();
    //     _cycles += 2;
    //     break;

    // // POP BC
    // case 0xC1:
    //     R.BC = PopWord();
    //     _cycles += 2;
    //     break;

    // // POP DE
    // case 0xD1:
    //     R.DE = PopWord();
    //     _cycles += 2;
    //     break;

    // // POP HL
    // case 0xE1:
    //     R.HL = PopWord();
    //     _cycles += 2;
    //     break;

    // // LD A,B
    // case 0x78:
    //     R.A = R.B;
    //     break;
        
    // // LD A,C
    // case 0x79:
    //     R.A = R.C;
    //     break;
        
    // // LD A,D
    // case 0x7A:
    //     R.A = R.D;
    //     break;
        
    // // LD A,E
    // case 0x7B:
    //     R.A = R.E;
    //     break;
        
    // // LD A,H
    // case 0x7C:
    //     R.A = R.H;
    //     break;
        
    // // LD A,L
    // case 0x7D:
    //     R.A = R.L;
    //     break;
        
    // // LD A,(HL)
    // case 0x7E:
    //     R.A = ReadByte(R.HL);
    //     _cycles += 1;
    //     break;
        
    // // LD A,A
    // case 0x7F:
    //     R.A = R.A;
    //     break;
        
    // // LD A,u8
    // case 0x3E:
    //     R.A = u8;
    //     ++R.PC;
    //     _cycles += 1;
    //     break;
        
    // // LD B,B
    // case 0x40:
    //     R.B = R.B;
    //     break;
        
    // // LD B,C
    // case 0x41:
    //     R.B = R.C;
    //     break;
        
    // // LD B,D
    // case 0x42:
    //     R.B = R.D;
    //     break;
        
    // // LD B,E
    // case 0x43:
    //     R.B = R.E;
    //     break;
        
    // // LD B,H
    // case 0x44:
    //     R.B = R.H;
    //     break;
        
    // // LD B,L
    // case 0x45:
    //     R.B = R.L;
    //     break;
        
    // // LD B,(HL)
    // case 0x46:
    //     R.B = ReadByte(R.HL);
    //     _cycles += 1;
    //     break;
        
    // // LD B,A
    // case 0x47:
    //     R.B = R.A;
    //     break;
        
    // // LD B,u8
    // case 0x06:
    //     R.B = u8;
    //     ++R.PC;
    //     _cycles += 1;
    //     break;
        
    // // LD C,B
    // case 0x48:
    //     R.C = R.B;
    //     break;
        
    // // LD C,C
    // case 0x49:
    //     R.C = R.C;
    //     break;
        
    // // LD C,D
    // case 0x4A:
    //     R.C = R.D;
    //     break;
        
    // // LD C,E
    // case 0x4B:
    //     R.C = R.E;
    //     break;
        
    // // LD C,H
    // case 0x4C:
    //     R.C = R.H;
    //     break;
        
    // // LD C,L
    // case 0x4D:
    //     R.C = R.L;
    //     break;
        
    // // LD C,(HL)
    // case 0x4E:
    //     R.C = ReadByte(R.HL);
    //     _cycles += 1;
    //     break;
        
    // // LD C,A
    // case 0x4F:
    //     R.C = R.A;
    //     break;
        
    // // LD C,u8
    // case 0x0E:
    //     R.C = u8;
    //     ++R.PC;
    //     _cycles += 1;
    //     break;
        
    // // LD D,B
    // case 0x50:
    //     R.D = R.B;
    //     break;
        
    // // LD D,C
    // case 0x51:
    //     R.D = R.C;
    //     break;
        
    // // LD D,D
    // case 0x52:
    //     R.D = R.D;
    //     break;
        
    // // LD D,E
    // case 0x53:
    //     R.D = R.E;
    //     break;
        
    // // LD D,H
    // case 0x54:
    //     R.D = R.H;
    //     break;
        
    // // LD D,L
    // case 0x55:
    //     R.D = R.L;
    //     break;
        
    // // LD D,(HL)
    // case 0x56:
    //     R.D = ReadByte(R.HL);
    //     _cycles += 1;
    //     break;
        
    // // LD D,A
    // case 0x57:
    //     R.D = R.A;
    //     break;
        
    // // LD D,u8
    // case 0x16:
    //     R.D = u8;
    //     ++R.PC;
    //     _cycles += 1;
    //     break;
        
    // // LD E,B
    // case 0x58:
    //     R.E = R.B;
    //     break;
        
    // // LD E,C
    // case 0x59:
    //     R.E = R.C;
    //     break;
        
    // // LD E,D
    // case 0x5A:
    //     R.E = R.D;
    //     break;
        
    // // LD E,E
    // case 0x5B:
    //     R.E = R.E;
    //     break;
        
    // // LD E,H
    // case 0x5C:
    //     R.E = R.H;
    //     break;
        
    // // LD E,L
    // case 0x5D:
    //     R.E = R.L;
    //     break;
        
    // // LD E,(HL)
    // case 0x5E:
    //     R.E = ReadByte(R.HL);
    //     _cycles += 1;
    //     break;
        
    // // LD E,A
    // case 0x5F:
    //     R.E = R.A;
    //     break;
        
    // // LD E,u8
    // case 0x1E:
    //     R.E = u8;
    //     ++R.PC;
    //     _cycles += 1;
    //     break;
        
    // // LD H,B
    // case 0x60:
    //     R.H = R.B;
    //     break;
        
    // // LD H,C
    // case 0x61:
    //     R.H = R.C;
    //     break;
        
    // // LD H,D
    // case 0x62:
    //     R.H = R.D;
    //     break;
        
    // // LD H,E
    // case 0x63:
    //     R.H = R.E;
    //     break;
        
    // // LD H,H
    // case 0x64:
    //     R.H = R.H;
    //     break;
        
    // // LD H,L
    // case 0x65:
    //     R.H = R.L;
    //     break;
        
    // // LD H,(HL)
    // case 0x66:
    //     R.H = ReadByte(R.HL);
    //     _cycles += 1;
    //     break;
        
    // // LD H,A
    // case 0x67:
    //     R.H = R.A;
    //     break;
        
    // // LD H,u8
    // case 0x26:
    //     R.H = u8;
    //     ++R.PC;
    //     _cycles += 1;
    //     break;
        
    // // LD L,B
    // case 0x68:
    //     R.L = R.B;
    //     break;
        
    // // LD L,C
    // case 0x69:
    //     R.L = R.C;
    //     break;
        
    // // LD L,D
    // case 0x6A:
    //     R.L = R.D;
    //     break;
        
    // // LD L,E
    // case 0x6B:
    //     R.L = R.E;
    //     break;
        
    // // LD L,H
    // case 0x6C:
    //     R.L = R.H;
    //     break;
        
    // // LD L,L
    // case 0x6D:
    //     R.L = R.L;
    //     break;
        
    // // LD L,(HL)
    // case 0x6E:
    //     R.L = ReadByte(R.HL);
    //     _cycles += 1;
    //     break;
        
    // // LD L,A
    // case 0x6F:
    //     R.L = R.A;
    //     break;
        
    // // LD L,u8
    // case 0x2E:
    //     R.L = u8;
    //     ++R.PC;
    //     _cycles += 1;
    //     break;
    
    // // LD (HL),B
    // case 0x70:
    //     WriteByte(R.HL, R.B);
    //     _cycles += 1;
    //     break;

    // // LD (HL),C
    // case 0x71:
    //     WriteByte(R.HL, R.C);
    //     _cycles += 1;
    //     break;

    // // LD (HL),D
    // case 0x72:
    //     WriteByte(R.HL, R.D);
    //     _cycles += 1;
    //     break;

    // // LD (HL),E
    // case 0x73:
    //     WriteByte(R.HL, R.E);
    //     _cycles += 1;
    //     break;

    // // LD (HL),H
    // case 0x74:
    //     WriteByte(R.HL, R.H);
    //     _cycles += 1;
    //     break;

    // // LD (HL),L
    // case 0x75:
    //     WriteByte(R.HL, R.L);
    //     _cycles += 1;
    //     break;

    // // LD (HL),u8
    // case 0x36:
    //     ++R.PC;
    //     WriteByte(R.HL, u8);
    //     _cycles += 2;
    //     break;

    // // LD (HL),A
    // case 0x77:
    //     WriteByte(R.HL, R.A);
    //     _cycles += 1;
    //     break;

    // // LD (BC),A
    // case 0x02:
    //     WriteByte(R.BC, R.A);
    //     _cycles += 1;
    //     break;

    // // LD (DE),A
    // case 0x12:
    //     WriteByte(R.DE, R.A);
    //     _cycles += 1;
    //     break;

    // // LD A,(BC)
    // case 0x0A:
    //     R.A = ReadByte(R.BC);
    //     _cycles += 1;
    //     break;

    // // LD A,(DE)
    // case 0x1A:
    //     R.A = ReadByte(R.DE);
    //     _cycles += 1;
    //     break;

    // // LD BC,u16
    // case 0x01:
    //     R.PC += 2;
    //     R.BC = u16;
    //     _cycles += 2;
    //     break;

    // // LD DE,u16
    // case 0x11:
    //     R.PC += 2;
    //     R.DE = u16;
    //     _cycles += 2;
    //     break;

    // // LD HL,u16
    // case 0x21:
    //     R.PC += 2;
    //     R.HL = u16;
    //     _cycles += 2;
    //     break;

    // // LD SP,u16
    // case 0x31:
    //     R.PC += 2;
    //     R.SP = u16;
    //     _cycles += 2;
    //     break;

    // // LD SP,HL
    // case 0xF9:
    //     R.SP = R.HL;
    //     break;

    // // LD (u16),SP
    // case 0x08:
    //     R.PC += 2;
    //     WriteWord(u16, R.SP);
    //     _cycles += 4;
    //     break;

    // // LD (u16),A
    // case 0xEA:
    //     R.PC += 2;
    //     WriteByte(u16, R.A);
    //     _cycles += 3;
    //     break;

    // // LD A,(u16)
    // case 0xFA:
    //     R.PC += 2;
    //     R.A = ReadByte(u16);
    //     _cycles += 3;
    //     break;

    // // LD (HL+),A
    // // LD (HLI),A
    // // LDI (HL),A
    // case 0x22:
    //     WriteByte(R.HL, R.A);
    //     _cycles += 1;
    //     ++R.HL;
    //     break;

    // // LD (HL-),A
    // // LD (HLD),A
    // // LDD (HL),A
    // case 0x32:
    //     WriteByte(R.HL, R.A);
    //     _cycles += 1;
    //     --R.HL;
    //     break;

    // // LD A,(HL+)
    // // LD A,(HLI)
    // // LDI A,(HL)
    // case 0x2A:
    //     R.A = ReadByte(R.HL);
    //     _cycles += 1;
    //     ++R.HL;
    //     break;

    // // LD A,(HL-)
    // // LD A,(HLD)
    // // LDD A,(HL)
    // case 0x3A:
    //     R.A = ReadByte(R.HL);
    //     _cycles += 1;
    //     --R.HL;
    //     break;

    // // LD ($FF00+u8),A
    // // LDH (u8),A
    // case 0xE0:
    //     ++R.PC;
    //     WriteByte(0xFF00 + u8, R.A);
    //     _cycles += 2;
    //     break;

    // // LD A,($FF00+u8)
    // // LDH A,(u8)
    // case 0xF0:
    //     ++R.PC;
    //     R.A = ReadByte(0xFF00 + u8);
    //     _cycles += 2;
    //     break;

    // // LD ($FF00+C),A
    // // LDH (C),A
    // case 0xE2:
    //     WriteByte(0xFF00 + R.C, R.A);
    //     _cycles += 1;
    //     break;

    // // LD A,($FF00+C)
    // // LDH A,(C)
    // case 0xF2:
    //     R.A = ReadByte(0xFF00 + R.C);
    //     _cycles += 1;
    //     break;

    // // LD HL,SP+s8
    // case 0xF8:
    //     ++R.PC;
    //     u32 = R.SP + (int8_t)u8;
    //     R.FN = false;
    //     R.FH = ((((R.SP & 0x7FF) + (int8_t)u8) & 0x800) == 0x800);
    //     R.FC = ((u32 & 0xFFFF0000) > 0);
    //     R.HL = (uint16_t)u32;
    //     _cycles += 2;
    //     break;

    // #define _ADD(X)                                                             \
    //     u8 = (X);                                                               \
    //     u16 = R.A + u8;                                                         \
    //     R.FN = false;                                                          \
    //     R.FZ = ((u16 & 0xFF) == 0);                                            \
    //     R.FH = ((((R.A & 0x0F) + (u8 & 0x0F)) & 0x10) == 0x10);                \
    //     R.FC = ((u16 & 0xFF00) > 0);                                           \
    //     R.A = (uint16_t)u16

    // // ADD B
    // case 0x80:
    //     _ADD(R.B);
    //     break;

    // // ADD C
    // case 0x81:
    //     _ADD(R.C);
    //     break;

    // // ADD D
    // case 0x82:
    //     _ADD(R.D);
    //     break;

    // // ADD E
    // case 0x83:
    //     _ADD(R.E);
    //     break;

    // // ADD H
    // case 0x84:
    //     _ADD(R.H);
    //     break;

    // // ADD L
    // case 0x85:
    //     _ADD(R.L);
    //     break;

    // // ADD (HL)
    // case 0x86:
    //     _ADD(ReadByte(R.HL));
    //     _cycles += 1;
    //     break;

    // // ADD A
    // case 0x87:
    //     _ADD(R.A);
    //     break;

    // // ADD u8
    // case 0xC6:
    //     ++R.PC;
    //     _ADD(u8);
    //     _cycles += 1;
    //     break;

    // #define _ADC(X)                                                             \
    //     _ADD((X) + (R.FC ? 1 : 0))

    // // ADC B
    // case 0x88:
    //     _ADC(R.B);
    //     break;

    // // ADC C
    // case 0x89:
    //     _ADC(R.C);
    //     break;

    // // ADC D
    // case 0x8A:
    //     _ADC(R.D);
    //     break;

    // // ADC E
    // case 0x8B:
    //     _ADC(R.E);
    //     break;

    // // ADC H
    // case 0x8C:
    //     _ADC(R.H);
    //     break;

    // // ADC L
    // case 0x8D:
    //     _ADC(R.L);
    //     break;

    // // ADC (HL)
    // case 0x8E:
    //     _ADC(ReadByte(R.HL));
    //     _cycles += 1;
    //     break;

    // // ADC A
    // case 0x8F:
    //     _ADC(R.A);
    //     break;

    // // ADC u8
    // case 0xCE:
    //     ++R.PC;
    //     _ADC(u8);
    //     _cycles += 1;
    //     break;

    // #define _ADD_HL(X)                                                          \
    //     u16 = (X);                                                              \
    //     u32 = R.HL + u16;                                                       \
    //     R.FN = false;                                                          \
    //     R.FH = ((((R.A & 0x7FF) + (u16 & 0x7FF)) & 0x800) == 0x800);           \
    //     R.FC = ((u32 & 0xFFFF0000) > 0);                                       \
    //     R.HL = (uint16_t)u32

    // // ADD HL,BC
    // case 0x09:
    //     _ADD_HL(R.BC);
    //     break;

    // // ADD HL,DE
    // case 0x19:
    //     _ADD_HL(R.DE);
    //     break;

    // // ADD HL,HL
    // case 0x29:
    //     _ADD_HL(R.HL);
    //     break;

    // // ADD HL,SP
    // case 0x39:
    //     _ADD_HL(R.SP);
    //     break;

    // // ADD SP,s8
    // case 0xE8:
    //     ++R.PC;
    //     u32 = R.SP + (int8_t)u8;
    //     R.FN = false;
    //     R.FH = ((((R.SP & 0x7FF) + (int8_t)u8) & 0x800) == 0x800);
    //     R.FC = ((u32 & 0xFFFF0000) > 0);
    //     R.SP = (uint16_t)u32;
    //     _cycles += 3;
    //     break;

    // #define _CP(X)                                                              \
    //     u8 = (X);                                                               \
    //     u16 = R.A - u8;                                                         \
    //     R.FN = true;                                                           \
    //     R.FH = ((u8 & 0x0F) > (R.A & 0x0F));                                   \
    //     R.FC = ((u16 & 0xFF00) > 0);                                           \
    //     R.FZ = ((u16 & 0xFF) == 0)
    
    // // CP B
    // case 0xB8:
    //     _CP(R.B);
    //     break;

    // // CP C
    // case 0xB9:
    //     _CP(R.C);
    //     break;

    // // CP D
    // case 0xBA:
    //     _CP(R.D);
    //     break;

    // // CP E
    // case 0xBB:
    //     _CP(R.E);
    //     break;

    // // CP H
    // case 0xBC:
    //     _CP(R.H);
    //     break;

    // // CP L
    // case 0xBD:
    //     _CP(R.L);
    //     break;

    // // CP (HL)
    // case 0xBE:
    //     _CP(ReadByte(R.HL));
    //     _cycles += 1;
    //     break;

    // // CP A
    // case 0xBF:
    //     _CP(R.A);
    //     break;

    // // CP u8
    // case 0xFE:
    //     ++R.PC;
    //     _CP(u8);
    //     _cycles += 1;
    //     break;

    // #define _SUB(X)                                                             \
    //     _CP(X);                                                                 \
    //     R.A = (uint8_t)u16

    // // SUB B
    // case 0x90:
    //     _SUB(R.B);
    //     break;

    // // SUB C
    // case 0x91:
    //     _SUB(R.C);
    //     break;

    // // SUB D
    // case 0x92:
    //     _SUB(R.D);
    //     break;

    // // SUB E
    // case 0x93:
    //     _SUB(R.E);
    //     break;

    // // SUB H
    // case 0x94:
    //     _SUB(R.H);
    //     break;

    // // SUB L
    // case 0x95:
    //     _SUB(R.L);
    //     break;

    // // SUB (HL)
    // case 0x96:
    //     _SUB(ReadByte(R.HL));
    //     _cycles += 1;
    //     break;

    // // SUB A
    // case 0x97:
    //     _SUB(R.A);
    //     break;

    // // SUB u8
    // case 0xD6:
    //     ++R.PC;
    //     _SUB(u8);
    //     _cycles += 1;
    //     break;

    // #define _SBC(X)                                                             \
    //     _SUB((X) + (uint8_t)R.FC)

    // // SBC B
    // case 0x98:
    //     _SBC(R.B);
    //     break;

    // // SBC C
    // case 0x99:
    //     _SBC(R.C);
    //     break;

    // // SBC D
    // case 0x9A:
    //     _SBC(R.D);
    //     break;

    // // SBC E
    // case 0x9B:
    //     _SBC(R.E);
    //     break;

    // // SBC H
    // case 0x9C:
    //     _SBC(R.H);
    //     break;

    // // SBC L
    // case 0x9D:
    //     _SBC(R.L);
    //     break;

    // // SBC (HL)
    // case 0x9E:
    //     _SBC(ReadByte(R.HL));
    //     _cycles += 1;
    //     break;

    // // SBC A
    // case 0x9F:
    //     _SBC(R.A);
    //     break;

    // // SBC_u8
    // case 0xDE:
    //     ++R.PC;
    //     _SBC(u8);
    //     _cycles += 1;
    //     break;

    // #define _INC(VAR)                                                           \
    //     R.FH = (((VAR) & 0x0F) == 0x0F);                                       \
    //     ++(VAR);                                                                \
    //     R.FZ = ((VAR) == 0);                                                   \
    //     R.FN = false

    // // INC B
    // case 0x04:
    //     _INC(R.B);
    //     break;

    // // INC C
    // case 0x0C:
    //     _INC(R.C);
    //     break;

    // // INC D
    // case 0x14:
    //     _INC(R.D);
    //     break;

    // // INC E
    // case 0x1C:
    //     _INC(R.E);
    //     break;

    // // INC H
    // case 0x24:
    //     _INC(R.H);
    //     break;

    // // INC L
    // case 0x2C:
    //     _INC(R.L);
    //     break;

    // // INC (HL)
    // case 0x34:
    //     u8 = ReadByte(R.HL);
    //     _INC(u8);
    //     WriteByte(R.HL, u8);
    //     _cycles += 2;
    //     break;

    // // INC A
    // case 0x3C:
    //     _INC(R.A);
    //     break;

    // // INC BC
    // case 0x03:
    //     /* Tick 1 */
    //     ++R.BC;
    //     break;

    // // INC DE
    // case 0x13:
    //     /* Tick 1 */
    //     ++R.DE;
    //     break;

    // // INC HL
    // case 0x23:
    //     /* Tick 1 */
    //     ++R.HL;
    //     break;

    // // INC SP
    // case 0x33:
    //     /* Tick 1 */
    //     ++R.SP;
    //     break;

    // #define _DEC(VAR)                                                           \
    //     R.FH = (((VAR) & 0x0F) == 0);                                          \
    //     --(VAR);                                                                \
    //     R.FZ = ((VAR) == 0);                                                   \
    //     R.FN = true

    // // DEC B
    // case 0x05:
    //     _DEC(R.B);
    //     break;

    // // DEC C
    // case 0x0D:
    //     _DEC(R.C);
    //     break;

    // // DEC D
    // case 0x15:
    //     _DEC(R.D);
    //     break;

    // // DEC E
    // case 0x1D:
    //     _DEC(R.E);
    //     break;

    // // DEC H
    // case 0x25:
    //     _DEC(R.H);
    //     break;

    // // DEC L
    // case 0x2D:
    //     _DEC(R.L);
    //     break;

    // // DEC (HL)
    // case 0x35:
    //     u8 = ReadByte(R.HL);
    //     _DEC(u8);
    //     WriteByte(R.HL, u8);
    //     _cycles += 2;
    //     break;

    // // DEC A
    // case 0x3D:
    //     _DEC(R.A);
    //     break;

    // // DEC BC
    // case 0x0B:
    //     --R.BC;
    //     break;

    // // DEC DE
    // case 0x1B:
    //     --R.DE;
    //     break;

    // // DEC HL
    // case 0x2B:
    //     --R.HL;
    //     break;

    // // DEC SP
    // case 0x3B:
    //     --R.SP;
    //     break;

    // #define _AND(X)                                                             \
    //     R.A &= (X);                                                             \
    //     R.FZ = (R.A == 0);                                                     \
    //     R.FN = false;                                                          \
    //     R.FH = true;                                                           \
    //     R.FC = false

    // // AND B
    // case 0xA0:
    //     _AND(R.B);
    //     break;

    // // AND C
    // case 0xA1:
    //     _AND(R.C);
    //     break;

    // // AND D
    // case 0xA2:
    //     _AND(R.D);
    //     break;

    // // AND E
    // case 0xA3:
    //     _AND(R.E);
    //     break;

    // // AND H
    // case 0xA4:
    //     _AND(R.H);
    //     break;

    // // AND L
    // case 0xA5:
    //     _AND(R.L);
    //     break;

    // // AND (HL)
    // case 0xA6:
    //     _AND(ReadByte(R.HL));
    //     _cycles += 1;
    //     break;

    // // AND A
    // case 0xA7:
    //     _AND(R.A);
    //     break;

    // // AND u8
    // case 0xE6:
    //     ++R.PC;
    //     _AND(u8);
    //     _cycles += 1;
    //     break;

    // #define _OR(X)                                                              \
    //     R.A |= (X);                                                             \
    //     R.FZ = (R.A == 0);                                                     \
    //     R.FN = false;                                                          \
    //     R.FH = false;                                                          \
    //     R.FC = false

    // // OR B
    // case 0xB0:
    //     _OR(R.B);
    //     break;

    // // OR C
    // case 0xB1:
    //     _OR(R.C);
    //     break;

    // // OR D
    // case 0xB2:
    //     _OR(R.D);
    //     break;

    // // OR E
    // case 0xB3:
    //     _OR(R.E);
    //     break;

    // // OR H
    // case 0xB4:
    //     _OR(R.H);
    //     break;

    // // OR L
    // case 0xB5:
    //     _OR(R.L);
    //     break;

    // // OR (HL)
    // case 0xB6:
    //     _OR(ReadByte(R.HL));
    //     _cycles += 1;
    //     break;

    // // OR A
    // case 0xB7:
    //     _OR(R.A);
    //     break;

    // // OR u8
    // case 0xF6:
    //     ++R.PC;
    //     _OR(u8);
    //     _cycles += 1;
    //     break;

    // #define _XOR(X)                                                             \
    //     R.A ^= (X);                                                             \
    //     R.FZ = (R.A == 0);                                                     \
    //     R.FN = false;                                                          \
    //     R.FH = false;                                                          \
    //     R.FC = false

    // // XOR B
    // case 0xA8:
    //     _XOR(R.B);
    //     break;

    // // XOR C
    // case 0xA9:
    //     _XOR(R.C);
    //     break;

    // // XOR D
    // case 0xAA:
    //     _XOR(R.D);
    //     break;

    // // XOR E
    // case 0xAB:
    //     _XOR(R.E);
    //     break;

    // // XOR H
    // case 0xAC:
    //     _XOR(R.H);
    //     break;

    // // XOR L
    // case 0xAD:
    //     _XOR(R.L);
    //     break;

    // // XOR (HL)
    // case 0xAE:
    //     _XOR(ReadByte(R.HL));
    //     _cycles += 1;
    //     break;

    // // XOR A
    // case 0xAF:
    //     _XOR(R.A);
    //     break;

    // // XOR u8
    // case 0xEE:
    //     ++R.PC;
    //     _XOR(u8);
    //     _cycles += 1;
    //     break;

    // #define _RLC(X)                                                             \
    //     R.FC = (((X) & 0x80) == 0x80);                                         \
    //     (X) = ((X) << 1) | (R.FC ? 1 : 0);                                     \
    //     R.FZ = ((X) == 0);                                                     \
    //     R.FN = false;                                                          \
    //     R.FH = false

    // // RLCA
    // case 0x07:
    //     _RLC(R.A);
    //     break;

    // #define _RRC(X)                                                             \
    //     R.FC = (((X) & 0x01) == 0x01);                                         \
    //     (X) = ((X) >> 1) | ((R.FC ? 1 : 0) << 7);                              \
    //     R.FZ = ((X) == 0);                                                     \
    //     R.FN = false;                                                          \
    //     R.FH = false

    // // RRCA
    // case 0x0F:
    //     _RRC(R.A);
    //     break;

    // #define _RL(X)                                                              \
    //     u8 = (((X) & 0x80) == 0x80);                                            \
    //     (X) = ((X) << 1) | (R.FC ? 1 : 0);                                     \
    //     R.FC = u8;                                                             \
    //     R.FZ = ((X) == 0);                                                     \
    //     R.FN = false;                                                          \
    //     R.FH = false

    // // RLA
    // case 0x17:
    //     _RL(R.A);
    //     break;

    // #define _RR(X)                                                              \
    //     u8 = (((X) & 0x01) == 0x01);                                            \
    //     (X) = ((X) >> 1) | ((R.FC ? 1 : 0) << 7);                              \
    //     R.FC = u8;                                                             \
    //     R.FZ = ((X) == 0);                                                     \
    //     R.FN = false;                                                          \
    //     R.FH = false

    // // RRA
    // case 0x1F:
    //     _RR(R.A);
    //     break;

    // case 0xCB:
    //     ++R.PC;
    //     _cycles += 1;

    //     switch (u8) {

    //     // RLC B
    //     case 0x00:
    //         _RLC(R.B);
    //         break;

    //     // RLC C
    //     case 0x01:
    //         _RLC(R.C);
    //         break;

    //     // RLC D
    //     case 0x02:
    //         _RLC(R.D);
    //         break;

    //     // RLC E
    //     case 0x03:
    //         _RLC(R.E);
    //         break;

    //     // RLC H
    //     case 0x04:
    //         _RLC(R.H);
    //         break;

    //     // RLC L
    //     case 0x05:
    //         _RLC(R.L);
    //         break;

    //     // RLC (HL)
    //     case 0x06:
    //         u8 = ReadByte(R.HL);
    //         _RLC(u8);
    //         WriteByte(R.HL, u8);
    //         _cycles += 2;
    //         break;

    //     // RLC A
    //     case 0x07:
    //         _RLC(R.A);
    //         break;

    //     // RRC B
    //     case 0x08:
    //         _RRC(R.B);
    //         break;

    //     // RRC C
    //     case 0x09:
    //         _RRC(R.C);
    //         break;

    //     // RRC D
    //     case 0x0A:
    //         _RRC(R.D);
    //         break;

    //     // RRC E
    //     case 0x0B:
    //         _RRC(R.E);
    //         break;

    //     // RRC H
    //     case 0x0C:
    //         _RRC(R.H);
    //         break;

    //     // RRC L
    //     case 0x0D:
    //         _RRC(R.L);
    //         break;

    //     // RRC (HL)
    //     case 0x0E:
    //         u8 = ReadByte(R.HL);
    //         _RRC(u8);
    //         WriteByte(R.HL, u8);
    //         break;

    //     // RRC A
    //     case 0x0F:
    //         _RRC(R.A);
    //         break;

    //     // RL B
    //     case 0x10:
    //         _RL(R.B);
    //         break;

    //     // RL C
    //     case 0x11:
    //         _RL(R.C);
    //         break;

    //     // RL D
    //     case 0x12:
    //         _RL(R.D);
    //         break;

    //     // RL E
    //     case 0x13:
    //         _RL(R.E);
    //         break;

    //     // RL H
    //     case 0x14:
    //         _RL(R.H);
    //         break;

    //     // RL L
    //     case 0x15:
    //         _RL(R.L);
    //         break;

    //     // RL (HL)
    //     case 0x16:
    //         u8 = ReadByte(R.HL);
    //         _RL(u8);
    //         WriteByte(R.HL, u8);
    //         _cycles += 2;
    //         break;

    //     // RL A
    //     case 0x17:
    //         _RL(R.A);
    //         break;

    //     // RR B
    //     case 0x18:
    //         _RR(R.B);
    //         break;

    //     // RR C
    //     case 0x19:
    //         _RR(R.C);
    //         break;

    //     // RR D
    //     case 0x1A:
    //         _RR(R.D);
    //         break;

    //     // RR E
    //     case 0x1B:
    //         _RR(R.E);
    //         break;

    //     // RR H
    //     case 0x1C:
    //         _RR(R.H);
    //         break;

    //     // RR L
    //     case 0x1D:
    //         _RR(R.L);
    //         break;

    //     // R (HL)
    //     case 0x1E:
    //         u8 = ReadByte(R.HL);
    //         _RR(u8);
    //         WriteByte(R.HL, u8);
    //         _cycles += 2;
    //         break;

    //     // RR A
    //     case 0x1F:
    //         _RR(R.A);
    //         break;

    //     #define _SLA(X)                                                         \
    //         R.FC = ((X)&0x80) == 0x80;                                         \
    //         (X) <<= 1;                                                          \
    //         R.FZ = ((X) == 0);                                                 \
    //         R.FN = false;                                                      \
    //         R.FH = false

    //     // SLA B
    //     case 0x20:
    //         _SLA(R.B);
    //         break;

    //     // SLA C
    //     case 0x21:
    //         _SLA(R.C);
    //         break;

    //     // SLA D
    //     case 0x22:
    //         _SLA(R.D);
    //         break;

    //     // SLA E
    //     case 0x23:
    //         _SLA(R.E);
    //         break;

    //     // SLA H
    //     case 0x24:
    //         _SLA(R.H);
    //         break;

    //     // SLA L
    //     case 0x25:
    //         _SLA(R.L);
    //         break;

    //     // SLA (HL)
    //     case 0x26:
    //         u8 = ReadByte(R.HL);
    //         _SLA(u8);
    //         WriteByte(R.HL, u8);
    //         _cycles += 2;
    //         break;

    //     // SLA A
    //     case 0x27:
    //         _SLA(R.A);
    //         break;

    //     #define _SRA(X)                                                         \
    //         R.FC = ((X)&0x01) == 0x01;                                         \
    //         (X) = ((X) >> 1) | ((X)&0x80);                                      \
    //         R.FZ = ((X) == 0);                                                 \
    //         R.FN = false;                                                      \
    //         R.FH = false
        
    //     // SRA B
    //     case 0x28:
    //         _SRA(R.B);
    //         break;

    //     // SRA C
    //     case 0x29:
    //         _SRA(R.C);
    //         break;

    //     // SRA D
    //     case 0x2A:
    //         _SRA(R.D);
    //         break;

    //     // SRA E
    //     case 0x2B:
    //         _SRA(R.E);
    //         break;

    //     // SRA H
    //     case 0x2C:
    //         _SRA(R.H);
    //         break;

    //     // SRA L
    //     case 0x2D:
    //         _SRA(R.L);
    //         break;

    //     // SRA (HL)
    //     case 0x2E:
    //         u8 = ReadByte(R.HL);
    //         _SRA(u8);
    //         WriteByte(R.HL, u8);
    //         _cycles += 2;
    //         break;

    //     // SRA A
    //     case 0x2F:
    //         _SRA(R.A);
    //         break;

    //     #define _SRL(X)                                                         \
    //         R.FC = ((X)&0x01) == 0x01;                                         \
    //         (X) >>= 1;                                                          \
    //         R.FZ = ((X) == 0);                                                 \
    //         R.FN = false;                                                      \
    //         R.FH = false

    //     // SRL B
    //     case 0x38:
    //         _SRL(R.B);
    //         break;

    //     // SRL C
    //     case 0x39:
    //         _SRL(R.C);
    //         break;

    //     // SRL D
    //     case 0x3A:
    //         _SRL(R.D);
    //         break;

    //     // SRL E
    //     case 0x3B:
    //         _SRL(R.E);
    //         break;

    //     // SRL H
    //     case 0x3C:
    //         _SRL(R.H);
    //         break;

    //     // SRL L
    //     case 0x3D:
    //         _SRL(R.L);
    //         break;

    //     // SRL (HL)
    //     case 0x3E:
    //         u8 = ReadByte(R.HL);
    //         _SRL(u8);
    //         WriteByte(R.HL, u8);
    //         _cycles += 2;
    //         break;

    //     // SRL A
    //     case 0x3F:
    //         _SRL(R.A);
    //         break;

    //     #define _SWAP(X)                                                        \
    //         (X) = (((X)&0x0F) << 4) | (((X)&0xF0) >> 4);                        \
    //         R.FZ = ((X) == 0);                                                 \
    //         R.FN = false;                                                      \
    //         R.FH = false;                                                      \
    //         R.FC = false

    //     // SWAP B
    //     case 0x30:
    //         _SWAP(R.B);
    //         break;

    //     // SWAP C
    //     case 0x31:
    //         _SWAP(R.C);
    //         break;

    //     // SWAP D
    //     case 0x32:
    //         _SWAP(R.D);
    //         break;

    //     // SWAP E
    //     case 0x33:
    //         _SWAP(R.E);
    //         break;

    //     // SWAP H
    //     case 0x34:
    //         _SWAP(R.H);
    //         break;

    //     // SWAP L
    //     case 0x35:
    //         _SWAP(R.L);
    //         break;

    //     // SWAP (HL)
    //     case 0x36:
    //         u8 = ReadByte(R.HL);
    //         _SWAP(u8);
    //         WriteByte(R.HL, u8);
    //         _cycles += 2;
    //         break;

    //     // SWAP A
    //     case 0x37:
    //         _SWAP(R.A);
    //         break;

    //     #define _BIT(NUM, X)                                                    \
    //         R.FZ = (((X) & (1 << NUM)) == 0);                                  \
    //         R.FN = false;                                                      \
    //         R.FH = true

    //     #define _RES(NUM, X)                                                    \
    //         X &= ~(1 << NUM)

    //     #define _BIT_GROUP(NUM)                                                 \
    //         case (0x40 + ((NUM) * 8) + 0):                                      \
    //             _BIT((NUM), R.B);                                               \
    //             break;                                                          \
    //         case (0x40 + ((NUM) * 8) + 1):                                      \
    //             _BIT((NUM), R.C);                                               \
    //             break;                                                          \
    //         case (0x40 + ((NUM) * 8) + 2):                                      \
    //             _BIT((NUM), R.D);                                               \
    //             break;                                                          \
    //         case (0x40 + ((NUM) * 8) + 3):                                      \
    //             _BIT((NUM), R.E);                                               \
    //             break;                                                          \
    //         case (0x40 + ((NUM) * 8) + 4):                                      \
    //             _BIT((NUM), R.H);                                               \
    //             break;                                                          \
    //         case (0x40 + ((NUM) * 8) + 5):                                      \
    //             _BIT((NUM), R.L);                                               \
    //             break;                                                          \
    //         case (0x40 + ((NUM) * 8) + 6):                                      \
    //             _BIT((NUM), ReadByte(R.HL));                                    \
    //             _cycles += 1;                                                   \
    //             break;                                                          \
    //         case (0x40 + ((NUM) * 8) + 7):                                      \
    //             _BIT((NUM), R.A);                                               \
    //             break;

    //     _BIT_GROUP(0)
    //     _BIT_GROUP(1)
    //     _BIT_GROUP(2)
    //     _BIT_GROUP(3)
    //     _BIT_GROUP(4)
    //     _BIT_GROUP(5)
    //     _BIT_GROUP(6)
    //     _BIT_GROUP(7)

    //     #define _RES_GROUP(NUM)                                                 \
    //         case (0x80 + ((NUM) * 8) + 0):                                      \
    //             _RES((NUM), R.B);                                               \
    //             break;                                                          \
    //         case (0x80 + ((NUM) * 8) + 1):                                      \
    //             _RES((NUM), R.C);                                               \
    //             break;                                                          \
    //         case (0x80 + ((NUM) * 8) + 2):                                      \
    //             _RES((NUM), R.D);                                               \
    //             break;                                                          \
    //         case (0x80 + ((NUM) * 8) + 3):                                      \
    //             _RES((NUM), R.E);                                               \
    //             break;                                                          \
    //         case (0x80 + ((NUM) * 8) + 4):                                      \
    //             _RES((NUM), R.H);                                               \
    //             break;                                                          \
    //         case (0x80 + ((NUM) * 8) + 5):                                      \
    //             _RES((NUM), R.L);                                               \
    //             break;                                                          \
    //         case (0x80 + ((NUM) * 8) + 6):                                      \
    //             u8 = ReadByte(R.HL);                                            \
    //             _RES((NUM), u8);                                                \
    //             WriteByte(R.HL, u8);                                            \
    //             _cycles += 2;                                                   \
    //             break;                                                          \
    //         case (0x80 + ((NUM) * 8) + 7):                                      \
    //             _RES((NUM), R.A);                                               \
    //             break;

    //     _RES_GROUP(0)
    //     _RES_GROUP(1)
    //     _RES_GROUP(2)
    //     _RES_GROUP(3)
    //     _RES_GROUP(4)
    //     _RES_GROUP(5)
    //     _RES_GROUP(6)
    //     _RES_GROUP(7)

    //     #define _SET(N, X)                                                      \
    //         X |= (1 << N)

    //     #define _SET_GROUP(NUM)                                                 \
    //         case (0xC0 + ((NUM) * 8) + 0):                                      \
    //             _SET((NUM), R.B);                                               \
    //             break;                                                          \
    //         case (0xC0 + ((NUM) * 8) + 1):                                      \
    //             _SET((NUM), R.C);                                               \
    //             break;                                                          \
    //         case (0xC0 + ((NUM) * 8) + 2):                                      \
    //             _SET((NUM), R.D);                                               \
    //             break;                                                          \
    //         case (0xC0 + ((NUM) * 8) + 3):                                      \
    //             _SET((NUM), R.E);                                               \
    //             break;                                                          \
    //         case (0xC0 + ((NUM) * 8) + 4):                                      \
    //             _SET((NUM), R.H);                                               \
    //             break;                                                          \
    //         case (0xC0 + ((NUM) * 8) + 5):                                      \
    //             _SET((NUM), R.L);                                               \
    //             break;                                                          \
    //         case (0xC0 + ((NUM) * 8) + 6):                                      \
    //             u8 = ReadByte(R.HL);                                            \
    //             _SET((NUM), u8);                                                \
    //             WriteByte(R.HL, u8);                                            \
    //             _cycles += 2;                                                   \
    //             break;                                                          \
    //         case (0xC0 + ((NUM) * 8) + 7):                                      \
    //             _SET((NUM), R.A);                                               \
    //             break;

    //     _SET_GROUP(0)
    //     _SET_GROUP(1)
    //     _SET_GROUP(2)
    //     _SET_GROUP(3)
    //     _SET_GROUP(4)
    //     _SET_GROUP(5)
    //     _SET_GROUP(6)
    //     _SET_GROUP(7)

    //     }

    //     break;
    // default:
    //     // Error
    //     break;
    // }

    // if (willEnableIME) {
    //     IME = true;
    //     RequestIME = false;
    // }

    // checkInterrupts();
}

} // namespace DMG_CPU
} // namespace kiwi