#include <Kiwi/DMG-CPU/Processor.hpp>
#include <Kiwi/Utility/Log.hpp>

namespace kiwi {
namespace DMG_CPU {

/*
const char * CB_instructionDefinitions[] = {
    // rotate
    [0x00] = "RLC B",
    [0x01] = "RLC C",
    [0x02] = "RLC D",
    [0x03] = "RLC E",
    [0x04] = "RLC H",
    [0x05] = "RLC L",
    [0x06] = "RLC (HL)",
    [0x07] = "RLC A",
    [0x08] = "RRC B",
    [0x09] = "RRC C",
    [0x0A] = "RRC D",
    [0x0B] = "RRC E",
    [0x0C] = "RRC H",
    [0x0D] = "RRC L",
    [0x0E] = "RRC (HL)",
    [0x0F] = "RRC A",
    [0x10] = "RL B",
    [0x11] = "RL C",
    [0x12] = "RL D",
    [0x13] = "RL E",
    [0x14] = "RL H",
    [0x15] = "RL L",
    [0x16] = "RL (HL)",
    [0x17] = "RL A",
    [0x18] = "RR B",
    [0x19] = "RR C",
    [0x1A] = "RR D",
    [0x1B] = "RR E",
    [0x1C] = "RR H",
    [0x1D] = "RR L",
    [0x1E] = "RR (HL)",
    [0x1F] = "RR A",
    // shift
    [0x20] = "SLA B",
    [0x21] = "SLA C",
    [0x22] = "SLA D",
    [0x23] = "SLA E",
    [0x24] = "SLA H",
    [0x25] = "SLA L",
    [0x26] = "SLA (HL)",
    [0x27] = "SLA A",
    [0x28] = "SRA B",
    [0x29] = "SRA C",
    [0x2A] = "SRA D",
    [0x2B] = "SRA E",
    [0x2C] = "SRA H",
    [0x2D] = "SRA L",
    [0x2E] = "SRA (HL)",
    [0x2F] = "SRA A",
    [0x30] = "SWAP B",
    [0x31] = "SWAP C",
    [0x32] = "SWAP D",
    [0x33] = "SWAP E",
    [0x34] = "SWAP H",
    [0x35] = "SWAP L",
    [0x36] = "SWAP (HL)",
    [0x37] = "SWAP A",
    [0x38] = "SRL B",
    [0x39] = "SRL C",
    [0x3A] = "SRL D",
    [0x3B] = "SRL E",
    [0x3C] = "SRL H",
    [0x3D] = "SRL L",
    [0x3E] = "SRL (HL)",
    [0x3F] = "SRL A",
};

// $b = bit

const char * bit_instructionDefinitions[] = {
    "BIT $b, B",
    "BIT $b, C",
    "BIT $b, D",
    "BIT $b, E",
    "BIT $b, H",
    "BIT $b, L",
    "BIT $b, (HL)",
    "BIT $b, A",
    "RES $b, B",
    "RES $b, C",
    "RES $b, D",
    "RES $b, E",
    "RES $b, H",
    "RES $b, L",
    "RES $b, (HL)",
    "RES $b, A",
    "SET $b, B",
    "SET $b, C",
    "SET $b, D",
    "SET $b, E",
    "SET $b, H",
    "SET $b, L",
    "SET $b, (HL)",
    "SET $b, A",
};
*/

String Disassemble(uint8_t opcode, uint8_t u8, uint16_t u16)
{
    static const char * instructionFormats[256] = { NULL };
    if (!instructionFormats[0]) {
        instructionFormats[0xCB] = "CB ...";
        // misc
        instructionFormats[0x00] = "NOP";
        instructionFormats[0x76] = "HALT";
        instructionFormats[0x10] = "STOP";
        instructionFormats[0xF3] = "DI";
        instructionFormats[0xFB] = "EI";
        instructionFormats[0x27] = "DAA";
        instructionFormats[0x37] = "SCF";
        instructionFormats[0x2F] = "CPL";
        instructionFormats[0x3F] = "CCF";
        // call
        instructionFormats[0xCD] = "CALL {2:04X}";
        instructionFormats[0xC4] = "CALL NZ, {2:04X}";
        instructionFormats[0xCC] = "CALL Z, {2:04X}";
        instructionFormats[0xD4] = "CALL NC, {2:04X}";
        instructionFormats[0xDC] = "CALL C, {2:04X}";
        // restart
        instructionFormats[0xC7] = "RST 00";
        instructionFormats[0xCF] = "RST 08";
        instructionFormats[0xD7] = "RST 10";
        instructionFormats[0xDF] = "RST 18";
        instructionFormats[0xE7] = "RST 20";
        instructionFormats[0xEF] = "RST 28";
        instructionFormats[0xF7] = "RST 30";
        instructionFormats[0xFF] = "RST 38";
        // return
        instructionFormats[0xC9] = "RET";
        instructionFormats[0xC0] = "RET NZ";
        instructionFormats[0xC8] = "RET Z";
        instructionFormats[0xD0] = "RET NC";
        instructionFormats[0xD8] = "RET C";
        instructionFormats[0xD9] = "RETI";
        // rotate
        instructionFormats[0x07] = "RLCA";
        instructionFormats[0x0F] = "RRCA";
        instructionFormats[0x17] = "RLA";
        instructionFormats[0x1F] = "RRA";
        // jump
        instructionFormats[0xC3] = "JP {2:04X}";
        instructionFormats[0xC2] = "JP NZ, {2:04X}";
        instructionFormats[0xCA] = "JP Z, {2:04X}";
        instructionFormats[0xD2] = "JP NC, {2:04X}";
        instructionFormats[0xDA] = "JP C, {2:04X}";
        instructionFormats[0x18] = "JR {1}";
        instructionFormats[0x20] = "JR NZ, {1}";
        instructionFormats[0x28] = "JR Z, {1}";
        instructionFormats[0x30] = "JR NC, {1}";
        instructionFormats[0x38] = "JR C, {1}";
        instructionFormats[0xE9] = "JP HL";
        // stack
        instructionFormats[0xF5] = "PUSH AF";
        instructionFormats[0xC5] = "PUSH BC";
        instructionFormats[0xD5] = "PUSH DE";
        instructionFormats[0xE5] = "PUSH HL";
        instructionFormats[0xF1] = "POP AF";
        instructionFormats[0xC1] = "POP BC";
        instructionFormats[0xD1] = "POP DE";
        instructionFormats[0xE1] = "POP HL";
        // load
        instructionFormats[0x40] = "LD B, B";
        instructionFormats[0x41] = "LD B, C";
        instructionFormats[0x42] = "LD B, D";
        instructionFormats[0x43] = "LD B, E";
        instructionFormats[0x44] = "LD B, H";
        instructionFormats[0x45] = "LD B, L";
        instructionFormats[0x46] = "LD B, (HL)";
        instructionFormats[0x47] = "LD B, A";
        instructionFormats[0x48] = "LD C, B";
        instructionFormats[0x49] = "LD C, C";
        instructionFormats[0x4A] = "LD C, D";
        instructionFormats[0x4B] = "LD C, E";
        instructionFormats[0x4C] = "LD C, H";
        instructionFormats[0x4D] = "LD C, L";
        instructionFormats[0x4E] = "LD C, (HL)";
        instructionFormats[0x4F] = "LD C, A";
        instructionFormats[0x50] = "LD D, B";
        instructionFormats[0x51] = "LD D, C";
        instructionFormats[0x52] = "LD D, D";
        instructionFormats[0x53] = "LD D, E";
        instructionFormats[0x54] = "LD D, H";
        instructionFormats[0x55] = "LD D, L";
        instructionFormats[0x56] = "LD D, (HL)";
        instructionFormats[0x57] = "LD D, A";
        instructionFormats[0x58] = "LD E, B";
        instructionFormats[0x59] = "LD E, C";
        instructionFormats[0x5A] = "LD E, D";
        instructionFormats[0x5B] = "LD E, E";
        instructionFormats[0x5C] = "LD E, H";
        instructionFormats[0x5D] = "LD E, L";
        instructionFormats[0x5E] = "LD E, (HL)";
        instructionFormats[0x5F] = "LD E, A";
        instructionFormats[0x60] = "LD H, B";
        instructionFormats[0x61] = "LD H, C";
        instructionFormats[0x62] = "LD H, D";
        instructionFormats[0x63] = "LD H, E";
        instructionFormats[0x64] = "LD H, H";
        instructionFormats[0x65] = "LD H, L";
        instructionFormats[0x66] = "LD H, (HL)";
        instructionFormats[0x67] = "LD H, A";
        instructionFormats[0x68] = "LD L, B";
        instructionFormats[0x69] = "LD L, C";
        instructionFormats[0x6A] = "LD L, D";
        instructionFormats[0x6B] = "LD L, E";
        instructionFormats[0x6C] = "LD L, H";
        instructionFormats[0x6D] = "LD L, L";
        instructionFormats[0x6E] = "LD L, (HL)";
        instructionFormats[0x6F] = "LD L, A";
        instructionFormats[0x70] = "LD (HL), B";
        instructionFormats[0x71] = "LD (HL), C";
        instructionFormats[0x72] = "LD (HL), D";
        instructionFormats[0x73] = "LD (HL), E";
        instructionFormats[0x74] = "LD (HL), H";
        instructionFormats[0x75] = "LD (HL), L";
        instructionFormats[0x77] = "LD (HL), A";
        instructionFormats[0x78] = "LD A, B";
        instructionFormats[0x79] = "LD A, C";
        instructionFormats[0x7A] = "LD A, D";
        instructionFormats[0x7B] = "LD A, E";
        instructionFormats[0x7C] = "LD A, H";
        instructionFormats[0x7D] = "LD A, L";
        instructionFormats[0x7E] = "LD A, (HL)";
        instructionFormats[0x7F] = "LD A, A";
        instructionFormats[0x02] = "LD (BC), A";
        instructionFormats[0x12] = "LD (DE), A";
        instructionFormats[0x06] = "LD B, {0:02X}";
        instructionFormats[0x0E] = "LD C, {0:02X}";
        instructionFormats[0x16] = "LD D, {0:02X}";
        instructionFormats[0x1E] = "LD E, {0:02X}";
        instructionFormats[0x26] = "LD H, {0:02X}";
        instructionFormats[0x2E] = "LD L, {0:02X}";
        instructionFormats[0x36] = "LD (HL), {0:02X}";
        instructionFormats[0x3E] = "LD A, {0:02X}";
        instructionFormats[0x0A] = "LD A, (BC)";
        instructionFormats[0x1A] = "LD A, (DE)";
        instructionFormats[0x01] = "LD BC, {2:04X}";
        instructionFormats[0x11] = "LD DE, {2:04X}";
        instructionFormats[0x21] = "LD HL, {2:04X}";
        instructionFormats[0x31] = "LD SP, {2:04X}";
        instructionFormats[0xF9] = "LD SP, HL";
        instructionFormats[0x08] = "LD ({2:04X}), SP";
        instructionFormats[0xEA] = "LD ({2:04X}), A";
        instructionFormats[0xFA] = "LD A, ({2:04X})";
        instructionFormats[0x22] = "LDI (HL), A";
        instructionFormats[0x32] = "LDD (HL), A";
        instructionFormats[0x2A] = "LDI A, (HL)";
        instructionFormats[0x3A] = "LDD A, (HL)";
        instructionFormats[0xE0] = "LDH ({0:02X}), A";
        instructionFormats[0xF0] = "LDH A, ({0:02X})";
        instructionFormats[0xE2] = "LDH (C), A";
        instructionFormats[0xF2] = "LDH A, (C)";
        instructionFormats[0xF8] = "LD HL, SP+{1}";
        // add
        instructionFormats[0x80] = "ADD B";
        instructionFormats[0x81] = "ADD C";
        instructionFormats[0x82] = "ADD D";
        instructionFormats[0x83] = "ADD E";
        instructionFormats[0x84] = "ADD H";
        instructionFormats[0x85] = "ADD L";
        instructionFormats[0x86] = "ADD (HL)";
        instructionFormats[0x87] = "ADD A";
        instructionFormats[0xC6] = "ADD {0:02X}";
        instructionFormats[0x88] = "ADC B";
        instructionFormats[0x89] = "ADC C";
        instructionFormats[0x8A] = "ADC D";
        instructionFormats[0x8B] = "ADC E";
        instructionFormats[0x8C] = "ADC H";
        instructionFormats[0x8D] = "ADC L";
        instructionFormats[0x8E] = "ADC (HL)";
        instructionFormats[0x8F] = "ADC A";
        instructionFormats[0xCE] = "ADC {0:02X}";
        instructionFormats[0x09] = "ADD HL, BC";
        instructionFormats[0x19] = "ADD HL, DE";
        instructionFormats[0x29] = "ADD HL, HL";
        instructionFormats[0x39] = "ADD HL, SP";
        instructionFormats[0xE8] = "ADD SP, {1}";
        // subtract
        instructionFormats[0x90] = "SUB B";
        instructionFormats[0x91] = "SUB C";
        instructionFormats[0x92] = "SUB D";
        instructionFormats[0x93] = "SUB E";
        instructionFormats[0x94] = "SUB H";
        instructionFormats[0x95] = "SUB L";
        instructionFormats[0x96] = "SUB (HL)";
        instructionFormats[0x97] = "SUB A";
        instructionFormats[0xD6] = "SUB {0:02X}";
        instructionFormats[0x98] = "SBC B";
        instructionFormats[0x99] = "SBC C";
        instructionFormats[0x9A] = "SBC D";
        instructionFormats[0x9B] = "SBC E";
        instructionFormats[0x9C] = "SBC H";
        instructionFormats[0x9D] = "SBC L";
        instructionFormats[0x9E] = "SBC (HL)";
        instructionFormats[0x9F] = "SBC A";
        instructionFormats[0xDE] = "SBC {0:02X}";
        // inc
        instructionFormats[0x04] = "INC B";
        instructionFormats[0x0C] = "INC C";
        instructionFormats[0x14] = "INC D";
        instructionFormats[0x1C] = "INC E";
        instructionFormats[0x24] = "INC H";
        instructionFormats[0x2C] = "INC L";
        instructionFormats[0x34] = "INC (HL)";
        instructionFormats[0x3C] = "INC A";
        instructionFormats[0x03] = "INC BC";
        instructionFormats[0x13] = "INC DE";
        instructionFormats[0x23] = "INC HL";
        instructionFormats[0x33] = "INC SP";
        // dec
        instructionFormats[0x05] = "DEC B";
        instructionFormats[0x0D] = "DEC C";
        instructionFormats[0x15] = "DEC D";
        instructionFormats[0x1D] = "DEC E";
        instructionFormats[0x25] = "DEC H";
        instructionFormats[0x2D] = "DEC L";
        instructionFormats[0x35] = "DEC (HL)";
        instructionFormats[0x3D] = "DEC A";
        instructionFormats[0x0B] = "DEC BC";
        instructionFormats[0x1B] = "DEC DE";
        instructionFormats[0x2B] = "DEC HL";
        instructionFormats[0x3B] = "DEC SP";
        // compare
        instructionFormats[0xB8] = "CP B";
        instructionFormats[0xB9] = "CP C";
        instructionFormats[0xBA] = "CP D";
        instructionFormats[0xBB] = "CP E";
        instructionFormats[0xBC] = "CP H";
        instructionFormats[0xBD] = "CP L";
        instructionFormats[0xBE] = "CP (HL)";
        instructionFormats[0xBF] = "CP A";
        instructionFormats[0xFE] = "CP {0:02X}";
        // and
        instructionFormats[0xA0] = "AND B";
        instructionFormats[0xA1] = "AND C";
        instructionFormats[0xA2] = "AND D";
        instructionFormats[0xA3] = "AND E";
        instructionFormats[0xA4] = "AND H";
        instructionFormats[0xA5] = "AND L";
        instructionFormats[0xA6] = "AND (HL)";
        instructionFormats[0xA7] = "AND A";
        instructionFormats[0xE6] = "AND {0:02X}";
        // or
        instructionFormats[0xB0] = "OR B";
        instructionFormats[0xB1] = "OR C";
        instructionFormats[0xB2] = "OR D";
        instructionFormats[0xB3] = "OR E";
        instructionFormats[0xB4] = "OR H";
        instructionFormats[0xB5] = "OR L";
        instructionFormats[0xB6] = "OR (HL)";
        instructionFormats[0xB7] = "OR A";
        instructionFormats[0xF6] = "OR {0:02X}";
        // xor
        instructionFormats[0xA8] = "XOR B";
        instructionFormats[0xA9] = "XOR C";
        instructionFormats[0xAA] = "XOR D";
        instructionFormats[0xAB] = "XOR E";
        instructionFormats[0xAC] = "XOR H";
        instructionFormats[0xAD] = "XOR L";
        instructionFormats[0xAE] = "XOR (HL)";
        instructionFormats[0xAF] = "XOR A";
        instructionFormats[0xEE] = "XOR {0:02X}";
    }

    const char * format = instructionFormats[opcode];
    if (format) {
        return fmt::format(fmt::runtime(format), u8, (int8_t)u8, u16);
    }

    return String();
}

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
        // // _cycles += 5;
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
    if (!CPUEnabled) {
        // _cycles += 4;
        return;
    }

    // LastInstructionAddress = PC;

    bool willEnableIME = RequestIME;

    uint8_t u8;
    uint16_t u16;
    uint32_t u32;

    uint8_t opcode = readByte(PC);
    u8 = readByte(PC + 1);
    u16 = (readByte(PC + 2) << 8) | u8;

    // static bool fuck = false;
    // static int count = 0;
    // if (fuck || count > 100) {
    //     count = 0;

    //     Log(KIWI_ANCHOR,
    //         "{:010} AF={:04X} BC={:04X} DE={:04X} HL={:04X} SP={:04X} PC={:04X} {}",
    //         0, // _cycles,
    //         AF, BC, DE, HL, SP, PC,
    //         Disassemble(opcode, u8, u16)
    //     );
    // }
    // ++count;

    ++PC;
    // _cycles += 1;

    switch (opcode) {

    // NOP
    case 0x00:
        break;

    // STOP
    case 0x10:
        // Skip $00
        ++PC;
        // TODO: Improve
        break;

    // HALT
    case 0x76:
        CPUEnabled = false;
        // TODO: Improve
        break;

    // DI
    case 0xF3:
        IME = false;
        break;

    // EI
    case 0xFB:
        // Enabling IME is delayed by one instruction
        RequestIME = true;
        break;

    // DAA
    case 0x27:
        if (FN) {
            // Subtraction
            if (FC) {
                A -= 0x60;
            }
            if (FH) {
                A -= 0x06;
            }
        }
        else {
            // Addition
            if (FC || A > 0x99) {
                A += 0x60;
                FC = true;
            }
            if (FH || (A & 0x0F) > 0x09) {
                A += 0x06;
            }
        }

        FZ = (A == 0);
        FH = false;
        break;

    // SFC
    case 0x37:
        FN = false;
        FH = false;
        FC = true;
        break;

    // CCF
    case 0x3F:
        FN = false;
        FH = false;
        FC = !FC;
        break;

    // CPL
    case 0x2F:
        A = ~A;
        FN = true;
        FH = true;
        break;
    
    #define _CALL(COND)                                                         \
        PC += 2;                                                                \
        /* _cycles += 3; */                                                     \
        if (COND) {                                                             \
            pushWord(PC);                                                       \
            /* _cycles += 2; */                                                 \
            PC = u16;                                                           \
        }                                                                       \

    // CALL u16
    case 0xCD:
        _CALL(true);
        break;

    // CALL NZ,u16
    case 0xC4:
        _CALL(!FZ);
        break;

    // CALL Z,u16
    case 0xCC:
        _CALL(FZ);
        break;

    // CALL NC,u16
    case 0xD4:
        _CALL(!FC);
        break;

    // CALL C,u16
    case 0xDC:
        _CALL(FC);
        break;

    #define _RST(ADDR)                                                          \
        pushWord(PC);                                                           \
        /* _cycles += 3; */                                                     \
        PC = (ADDR)

    // RST $00
    case 0xC7:
        _RST(0x00);
        break;
    
    // RST $08
    case 0xCF:
        _RST(0x08);
        break;
    
    // RST $10
    case 0xD7:
        _RST(0x10);
        break;
    
    // RST $18
    case 0xDF:
        _RST(0x18);
        break;
    
    // RST $20
    case 0xE7:
        _RST(0x20);
        break;
    
    // RST $28
    case 0xEF:
        _RST(0x28);
        break;
    
    // RST $30
    case 0xF7:
        _RST(0x30);
        break;
    
    // RST $38
    case 0xFF:
        _RST(0x38);
        break;

    // RET
    case 0xC9:
        u16 = popWord();
        // _cycles += 3;
        PC = u16;
        break;

    // RETI
    case 0xD9:
        u16 = popWord();
        // _cycles += 3;
        PC = u16;
        IME = true;
        break;
    
    #define _RET(COND)                                                          \
        /* _cycles += 2; */                                                     \
        if (COND) {                                                             \
            u16 = popWord();                                                    \
            /* _cycles += 3; */                                                 \
            PC = u16;                                                           \
        }

    // RET NZ
    case 0xC0:
        _RET(!FZ);
        break;
    
    // RET Z
    case 0xC8:
        _RET(FZ);
        break;
    
    // RET NC
    case 0xD0:
        _RET(!FC);
        break;
    
    // RET C
    case 0xD8:
        _RET(FC);
        break;
    
    // JP (HL)
    case 0xE9:
        PC = HL;
        break;

    #define _JP(COND)                                                           \
        PC += 2;                                                                \
        /* _cycles += 2; */                                                     \
        if (COND) {                                                             \
            /* _cycles += 1; */                                                 \
            PC = u16;                                                           \
        }

    // JP u16
    case 0xC3:
        _JP(true);
        break;
    
    // JP NZ,u16
    case 0xC2:
        _JP(!FZ);
        break;
    
    // JP Z,u16
    case 0xCA:
        _JP(FZ);
        break;
    
    // JP NC,u16
    case 0xD2:
        _JP(!FC);
        break;
    
    // JP C,u16
    case 0xDA:
        _JP(FC);
        break;
    
    #define _JR(COND)                                                           \
        ++PC;                                                                   \
        /* _cycles += 1; */                                                     \
        if (COND) {                                                             \
            /* _cycles += 1; */                                                 \
            PC += (int8_t)u8;                                                   \
        }

    // Js8
    case 0x18:
        _JR(true);
        break;
    
    // JNZ,s8
    case 0x20:
        _JR(!FZ);
        break;
    
    // JZ,s8
    case 0x28:
        _JR(FZ);
        break;
    
    // JNC,s8
    case 0x30:
        _JR(!FC);
        break;
    
    // JC,s8
    case 0x38:
        _JR(FC);
        break;

    // PUSH AF
    case 0xF5:
        pushWord(AF);
        // _cycles += 2;
        break;
    
    // PUSH BC
    case 0xC5:
        pushWord(BC);
        // _cycles += 2;
        break;
    
    // PUSH DE
    case 0xD5:
        pushWord(DE);
        // _cycles += 2;
        break;
    
    // PUSH HL
    case 0xE5:
        pushWord(HL);
        // _cycles += 2;
        break;

    // POP AF
    case 0xF1:
        AF = popWord();
        // _cycles += 2;
        break;

    // POP BC
    case 0xC1:
        BC = popWord();
        // _cycles += 2;
        break;

    // POP DE
    case 0xD1:
        DE = popWord();
        // _cycles += 2;
        break;

    // POP HL
    case 0xE1:
        HL = popWord();
        // _cycles += 2;
        break;

    // LD A,B
    case 0x78:
        A = B;
        break;
        
    // LD A,C
    case 0x79:
        A = C;
        break;
        
    // LD A,D
    case 0x7A:
        A = D;
        break;
        
    // LD A,E
    case 0x7B:
        A = E;
        break;
        
    // LD A,H
    case 0x7C:
        A = H;
        break;
        
    // LD A,L
    case 0x7D:
        A = L;
        break;
        
    // LD A,(HL)
    case 0x7E:
        A = readByte(HL);
        // _cycles += 1;
        break;
        
    // LD A,A
    case 0x7F:
        A = A;
        break;
        
    // LD A,u8
    case 0x3E:
        A = u8;
        ++PC;
        // _cycles += 1;
        break;
        
    // LD B,B
    case 0x40:
        B = B;
        break;
        
    // LD B,C
    case 0x41:
        B = C;
        break;
        
    // LD B,D
    case 0x42:
        B = D;
        break;
        
    // LD B,E
    case 0x43:
        B = E;
        break;
        
    // LD B,H
    case 0x44:
        B = H;
        break;
        
    // LD B,L
    case 0x45:
        B = L;
        break;
        
    // LD B,(HL)
    case 0x46:
        B = readByte(HL);
        // _cycles += 1;
        break;
        
    // LD B,A
    case 0x47:
        B = A;
        break;
        
    // LD B,u8
    case 0x06:
        B = u8;
        ++PC;
        // _cycles += 1;
        break;
        
    // LD C,B
    case 0x48:
        C = B;
        break;
        
    // LD C,C
    case 0x49:
        C = C;
        break;
        
    // LD C,D
    case 0x4A:
        C = D;
        break;
        
    // LD C,E
    case 0x4B:
        C = E;
        break;
        
    // LD C,H
    case 0x4C:
        C = H;
        break;
        
    // LD C,L
    case 0x4D:
        C = L;
        break;
        
    // LD C,(HL)
    case 0x4E:
        C = readByte(HL);
        // _cycles += 1;
        break;
        
    // LD C,A
    case 0x4F:
        C = A;
        break;
        
    // LD C,u8
    case 0x0E:
        C = u8;
        ++PC;
        // _cycles += 1;
        break;
        
    // LD D,B
    case 0x50:
        D = B;
        break;
        
    // LD D,C
    case 0x51:
        D = C;
        break;
        
    // LD D,D
    case 0x52:
        D = D;
        break;
        
    // LD D,E
    case 0x53:
        D = E;
        break;
        
    // LD D,H
    case 0x54:
        D = H;
        break;
        
    // LD D,L
    case 0x55:
        D = L;
        break;
        
    // LD D,(HL)
    case 0x56:
        D = readByte(HL);
        // _cycles += 1;
        break;
        
    // LD D,A
    case 0x57:
        D = A;
        break;
        
    // LD D,u8
    case 0x16:
        D = u8;
        ++PC;
        // _cycles += 1;
        break;
        
    // LD E,B
    case 0x58:
        E = B;
        break;
        
    // LD E,C
    case 0x59:
        E = C;
        break;
        
    // LD E,D
    case 0x5A:
        E = D;
        break;
        
    // LD E,E
    case 0x5B:
        E = E;
        break;
        
    // LD E,H
    case 0x5C:
        E = H;
        break;
        
    // LD E,L
    case 0x5D:
        E = L;
        break;
        
    // LD E,(HL)
    case 0x5E:
        E = readByte(HL);
        // _cycles += 1;
        break;
        
    // LD E,A
    case 0x5F:
        E = A;
        break;
        
    // LD E,u8
    case 0x1E:
        E = u8;
        ++PC;
        // _cycles += 1;
        break;
        
    // LD H,B
    case 0x60:
        H = B;
        break;
        
    // LD H,C
    case 0x61:
        H = C;
        break;
        
    // LD H,D
    case 0x62:
        H = D;
        break;
        
    // LD H,E
    case 0x63:
        H = E;
        break;
        
    // LD H,H
    case 0x64:
        H = H;
        break;
        
    // LD H,L
    case 0x65:
        H = L;
        break;
        
    // LD H,(HL)
    case 0x66:
        H = readByte(HL);
        // _cycles += 1;
        break;
        
    // LD H,A
    case 0x67:
        H = A;
        break;
        
    // LD H,u8
    case 0x26:
        H = u8;
        ++PC;
        // _cycles += 1;
        break;
        
    // LD L,B
    case 0x68:
        L = B;
        break;
        
    // LD L,C
    case 0x69:
        L = C;
        break;
        
    // LD L,D
    case 0x6A:
        L = D;
        break;
        
    // LD L,E
    case 0x6B:
        L = E;
        break;
        
    // LD L,H
    case 0x6C:
        L = H;
        break;
        
    // LD L,L
    case 0x6D:
        L = L;
        break;
        
    // LD L,(HL)
    case 0x6E:
        L = readByte(HL);
        // _cycles += 1;
        break;
        
    // LD L,A
    case 0x6F:
        L = A;
        break;
        
    // LD L,u8
    case 0x2E:
        L = u8;
        ++PC;
        // _cycles += 1;
        break;
    
    // LD (HL),B
    case 0x70:
        writeByte(HL, B);
        // _cycles += 1;
        break;

    // LD (HL),C
    case 0x71:
        writeByte(HL, C);
        // _cycles += 1;
        break;

    // LD (HL),D
    case 0x72:
        writeByte(HL, D);
        // _cycles += 1;
        break;

    // LD (HL),E
    case 0x73:
        writeByte(HL, E);
        // _cycles += 1;
        break;

    // LD (HL),H
    case 0x74:
        writeByte(HL, H);
        // _cycles += 1;
        break;

    // LD (HL),L
    case 0x75:
        writeByte(HL, L);
        // _cycles += 1;
        break;

    // LD (HL),u8
    case 0x36:
        ++PC;
        writeByte(HL, u8);
        // _cycles += 2;
        break;

    // LD (HL),A
    case 0x77:
        writeByte(HL, A);
        // _cycles += 1;
        break;

    // LD (BC),A
    case 0x02:
        writeByte(BC, A);
        // _cycles += 1;
        break;

    // LD (DE),A
    case 0x12:
        writeByte(DE, A);
        // _cycles += 1;
        break;

    // LD A,(BC)
    case 0x0A:
        A = readByte(BC);
        // _cycles += 1;
        break;

    // LD A,(DE)
    case 0x1A:
        A = readByte(DE);
        // _cycles += 1;
        break;

    // LD BC,u16
    case 0x01:
        PC += 2;
        BC = u16;
        // _cycles += 2;
        break;

    // LD DE,u16
    case 0x11:
        PC += 2;
        DE = u16;
        // _cycles += 2;
        break;

    // LD HL,u16
    case 0x21:
        PC += 2;
        HL = u16;
        // _cycles += 2;
        break;

    // LD SP,u16
    case 0x31:
        PC += 2;
        SP = u16;
        // _cycles += 2;
        break;

    // LD SP,HL
    case 0xF9:
        SP = HL;
        break;

    // LD (u16),SP
    case 0x08:
        PC += 2;
        writeWord(u16, SP);
        // _cycles += 4;
        break;

    // LD (u16),A
    case 0xEA:
        PC += 2;
        writeByte(u16, A);
        // _cycles += 3;
        break;

    // LD A,(u16)
    case 0xFA:
        PC += 2;
        A = readByte(u16);
        // _cycles += 3;
        break;

    // LD (HL+),A
    // LD (HLI),A
    // LDI (HL),A
    case 0x22:
        writeByte(HL, A);
        // _cycles += 1;
        ++HL;
        break;

    // LD (HL-),A
    // LD (HLD),A
    // LDD (HL),A
    case 0x32:
        writeByte(HL, A);
        // _cycles += 1;
        --HL;
        break;

    // LD A,(HL+)
    // LD A,(HLI)
    // LDI A,(HL)
    case 0x2A:
        A = readByte(HL);
        // _cycles += 1;
        ++HL;
        break;

    // LD A,(HL-)
    // LD A,(HLD)
    // LDD A,(HL)
    case 0x3A:
        A = readByte(HL);
        // _cycles += 1;
        --HL;
        break;

    // LD ($FF00+u8),A
    // LDH (u8),A
    case 0xE0:
        ++PC;
        writeByte(0xFF00 + u8, A);
        // _cycles += 2;
        break;

    // LD A,($FF00+u8)
    // LDH A,(u8)
    case 0xF0:
        ++PC;
        A = readByte(0xFF00 + u8);
        // _cycles += 2;
        break;

    // LD ($FF00+C),A
    // LDH (C),A
    case 0xE2:
        writeByte(0xFF00 + C, A);
        // _cycles += 1;
        break;

    // LD A,($FF00+C)
    // LDH A,(C)
    case 0xF2:
        A = readByte(0xFF00 + C);
        // _cycles += 1;
        break;

    // LD HL,SP+s8
    case 0xF8:
        ++PC;
        u32 = SP + (int8_t)u8;
        FN = false;
        FH = ((((SP & 0x7FF) + (int8_t)u8) & 0x800) == 0x800);
        FC = ((u32 & 0xFFFF0000) > 0);
        HL = (uint16_t)u32;
        // _cycles += 2;
        break;

    #define _ADD(X)                                                             \
        u8 = (X);                                                               \
        u16 = A + u8;                                                           \
        FN = false;                                                             \
        FZ = ((u16 & 0xFF) == 0);                                               \
        FH = ((((A & 0x0F) + (u8 & 0x0F)) & 0x10) == 0x10);                     \
        FC = ((u16 & 0xFF00) > 0);                                              \
        A = (uint16_t)u16

    // ADD B
    case 0x80:
        _ADD(B);
        break;

    // ADD C
    case 0x81:
        _ADD(C);
        break;

    // ADD D
    case 0x82:
        _ADD(D);
        break;

    // ADD E
    case 0x83:
        _ADD(E);
        break;

    // ADD H
    case 0x84:
        _ADD(H);
        break;

    // ADD L
    case 0x85:
        _ADD(L);
        break;

    // ADD (HL)
    case 0x86:
        _ADD(readByte(HL));
        // _cycles += 1;
        break;

    // ADD A
    case 0x87:
        _ADD(A);
        break;

    // ADD u8
    case 0xC6:
        ++PC;
        _ADD(u8);
        // _cycles += 1;
        break;

    #define _ADC(X)                                                             \
        _ADD((X) + (FC ? 1 : 0))

    // ADC B
    case 0x88:
        _ADC(B);
        break;

    // ADC C
    case 0x89:
        _ADC(C);
        break;

    // ADC D
    case 0x8A:
        _ADC(D);
        break;

    // ADC E
    case 0x8B:
        _ADC(E);
        break;

    // ADC H
    case 0x8C:
        _ADC(H);
        break;

    // ADC L
    case 0x8D:
        _ADC(L);
        break;

    // ADC (HL)
    case 0x8E:
        _ADC(readByte(HL));
        // _cycles += 1;
        break;

    // ADC A
    case 0x8F:
        _ADC(A);
        break;

    // ADC u8
    case 0xCE:
        ++PC;
        _ADC(u8);
        // _cycles += 1;
        break;

    #define _ADD_HL(X)                                                          \
        u16 = (X);                                                              \
        u32 = HL + u16;                                                         \
        FN = false;                                                             \
        FH = ((((A & 0x7FF) + (u16 & 0x7FF)) & 0x800) == 0x800);                \
        FC = ((u32 & 0xFFFF0000) > 0);                                          \
        HL = (uint16_t)u32

    // ADD HL,BC
    case 0x09:
        _ADD_HL(BC);
        break;

    // ADD HL,DE
    case 0x19:
        _ADD_HL(DE);
        break;

    // ADD HL,HL
    case 0x29:
        _ADD_HL(HL);
        break;

    // ADD HL,SP
    case 0x39:
        _ADD_HL(SP);
        break;

    // ADD SP,s8
    case 0xE8:
        ++PC;
        u32 = SP + (int8_t)u8;
        FN = false;
        FH = ((((SP & 0x7FF) + (int8_t)u8) & 0x800) == 0x800);
        FC = ((u32 & 0xFFFF0000) > 0);
        SP = (uint16_t)u32;
        // _cycles += 3;
        break;

    #define _CP(X)                                                              \
        u8 = (X);                                                               \
        u16 = A - u8;                                                           \
        FN = true;                                                              \
        FH = ((u8 & 0x0F) > (A & 0x0F));                                        \
        FC = ((u16 & 0xFF00) > 0);                                              \
        FZ = ((u16 & 0xFF) == 0)
    
    // CP B
    case 0xB8:
        _CP(B);
        break;

    // CP C
    case 0xB9:
        _CP(C);
        break;

    // CP D
    case 0xBA:
        _CP(D);
        break;

    // CP E
    case 0xBB:
        _CP(E);
        break;

    // CP H
    case 0xBC:
        _CP(H);
        break;

    // CP L
    case 0xBD:
        _CP(L);
        break;

    // CP (HL)
    case 0xBE:
        _CP(readByte(HL));
        // _cycles += 1;
        break;

    // CP A
    case 0xBF:
        _CP(A);
        break;

    // CP u8
    case 0xFE:
        ++PC;
        _CP(u8);
        // _cycles += 1;
        break;

    #define _SUB(X)                                                             \
        _CP(X);                                                                 \
        A = (uint8_t)u16

    // SUB B
    case 0x90:
        _SUB(B);
        break;

    // SUB C
    case 0x91:
        _SUB(C);
        break;

    // SUB D
    case 0x92:
        _SUB(D);
        break;

    // SUB E
    case 0x93:
        _SUB(E);
        break;

    // SUB H
    case 0x94:
        _SUB(H);
        break;

    // SUB L
    case 0x95:
        _SUB(L);
        break;

    // SUB (HL)
    case 0x96:
        _SUB(readByte(HL));
        // _cycles += 1;
        break;

    // SUB A
    case 0x97:
        _SUB(A);
        break;

    // SUB u8
    case 0xD6:
        ++PC;
        _SUB(u8);
        // _cycles += 1;
        break;

    #define _SBC(X)                                                             \
        _SUB((X) + (uint8_t)FC)

    // SBC B
    case 0x98:
        _SBC(B);
        break;

    // SBC C
    case 0x99:
        _SBC(C);
        break;

    // SBC D
    case 0x9A:
        _SBC(D);
        break;

    // SBC E
    case 0x9B:
        _SBC(E);
        break;

    // SBC H
    case 0x9C:
        _SBC(H);
        break;

    // SBC L
    case 0x9D:
        _SBC(L);
        break;

    // SBC (HL)
    case 0x9E:
        _SBC(readByte(HL));
        // _cycles += 1;
        break;

    // SBC A
    case 0x9F:
        _SBC(A);
        break;

    // SBC_u8
    case 0xDE:
        ++PC;
        _SBC(u8);
        // _cycles += 1;
        break;

    #define _INC(VAR)                                                           \
        FH = (((VAR) & 0x0F) == 0x0F);                                          \
        ++(VAR);                                                                \
        FZ = ((VAR) == 0);                                                      \
        FN = false

    // INC B
    case 0x04:
        _INC(B);
        break;

    // INC C
    case 0x0C:
        _INC(C);
        break;

    // INC D
    case 0x14:
        _INC(D);
        break;

    // INC E
    case 0x1C:
        _INC(E);
        break;

    // INC H
    case 0x24:
        _INC(H);
        break;

    // INC L
    case 0x2C:
        _INC(L);
        break;

    // INC (HL)
    case 0x34:
        u8 = readByte(HL);
        _INC(u8);
        writeByte(HL, u8);
        // _cycles += 2;
        break;

    // INC A
    case 0x3C:
        _INC(A);
        break;

    // INC BC
    case 0x03:
        /* Tick 1 */
        ++BC;
        break;

    // INC DE
    case 0x13:
        /* Tick 1 */
        ++DE;
        break;

    // INC HL
    case 0x23:
        /* Tick 1 */
        ++HL;
        break;

    // INC SP
    case 0x33:
        /* Tick 1 */
        ++SP;
        break;

    #define _DEC(VAR)                                                           \
        FH = (((VAR) & 0x0F) == 0);                                             \
        --(VAR);                                                                \
        FZ = ((VAR) == 0);                                                      \
        FN = true

    // DEC B
    case 0x05:
        _DEC(B);
        break;

    // DEC C
    case 0x0D:
        _DEC(C);
        break;

    // DEC D
    case 0x15:
        _DEC(D);
        break;

    // DEC E
    case 0x1D:
        _DEC(E);
        break;

    // DEC H
    case 0x25:
        _DEC(H);
        break;

    // DEC L
    case 0x2D:
        _DEC(L);
        break;

    // DEC (HL)
    case 0x35:
        u8 = readByte(HL);
        _DEC(u8);
        writeByte(HL, u8);
        // _cycles += 2;
        break;

    // DEC A
    case 0x3D:
        _DEC(A);
        break;

    // DEC BC
    case 0x0B:
        --BC;
        break;

    // DEC DE
    case 0x1B:
        --DE;
        break;

    // DEC HL
    case 0x2B:
        --HL;
        break;

    // DEC SP
    case 0x3B:
        --SP;
        break;

    #define _AND(X)                                                             \
        A &= (X);                                                               \
        FZ = (A == 0);                                                          \
        FN = false;                                                             \
        FH = true;                                                              \
        FC = false

    // AND B
    case 0xA0:
        _AND(B);
        break;

    // AND C
    case 0xA1:
        _AND(C);
        break;

    // AND D
    case 0xA2:
        _AND(D);
        break;

    // AND E
    case 0xA3:
        _AND(E);
        break;

    // AND H
    case 0xA4:
        _AND(H);
        break;

    // AND L
    case 0xA5:
        _AND(L);
        break;

    // AND (HL)
    case 0xA6:
        _AND(readByte(HL));
        // _cycles += 1;
        break;

    // AND A
    case 0xA7:
        _AND(A);
        break;

    // AND u8
    case 0xE6:
        ++PC;
        _AND(u8);
        // _cycles += 1;
        break;

    #define _OR(X)                                                              \
        A |= (X);                                                               \
        FZ = (A == 0);                                                          \
        FN = false;                                                             \
        FH = false;                                                             \
        FC = false

    // OR B
    case 0xB0:
        _OR(B);
        break;

    // OR C
    case 0xB1:
        _OR(C);
        break;

    // OR D
    case 0xB2:
        _OR(D);
        break;

    // OR E
    case 0xB3:
        _OR(E);
        break;

    // OR H
    case 0xB4:
        _OR(H);
        break;

    // OR L
    case 0xB5:
        _OR(L);
        break;

    // OR (HL)
    case 0xB6:
        _OR(readByte(HL));
        // _cycles += 1;
        break;

    // OR A
    case 0xB7:
        _OR(A);
        break;

    // OR u8
    case 0xF6:
        ++PC;
        _OR(u8);
        // _cycles += 1;
        break;

    #define _XOR(X)                                                             \
        A ^= (X);                                                               \
        FZ = (A == 0);                                                          \
        FN = false;                                                             \
        FH = false;                                                             \
        FC = false

    // XOR B
    case 0xA8:
        _XOR(B);
        break;

    // XOR C
    case 0xA9:
        _XOR(C);
        break;

    // XOR D
    case 0xAA:
        _XOR(D);
        break;

    // XOR E
    case 0xAB:
        _XOR(E);
        break;

    // XOR H
    case 0xAC:
        _XOR(H);
        break;

    // XOR L
    case 0xAD:
        _XOR(L);
        break;

    // XOR (HL)
    case 0xAE:
        _XOR(readByte(HL));
        // _cycles += 1;
        break;

    // XOR A
    case 0xAF:
        _XOR(A);
        break;

    // XOR u8
    case 0xEE:
        ++PC;
        _XOR(u8);
        // _cycles += 1;
        break;

    #define _RLC(X)                                                             \
        FC = (((X) & 0x80) == 0x80);                                            \
        (X) = ((X) << 1) | (FC ? 1 : 0);                                        \
        FZ = ((X) == 0);                                                        \
        FN = false;                                                             \
        FH = false

    // RLCA
    case 0x07:
        _RLC(A);
        break;

    #define _RRC(X)                                                             \
        FC = (((X) & 0x01) == 0x01);                                            \
        (X) = ((X) >> 1) | ((FC ? 1 : 0) << 7);                                 \
        FZ = ((X) == 0);                                                        \
        FN = false;                                                             \
        FH = false

    // RRCA
    case 0x0F:
        _RRC(A);
        break;

    #define _RL(X)                                                              \
        u8 = (((X) & 0x80) == 0x80);                                            \
        (X) = ((X) << 1) | (FC ? 1 : 0);                                        \
        FC = u8;                                                                \
        FZ = ((X) == 0);                                                        \
        FN = false;                                                             \
        FH = false

    // RLA
    case 0x17:
        _RL(A);
        break;

    #define _RR(X)                                                              \
        u8 = (((X) & 0x01) == 0x01);                                            \
        (X) = ((X) >> 1) | ((FC ? 1 : 0) << 7);                                 \
        FC = u8;                                                                \
        FZ = ((X) == 0);                                                        \
        FN = false;                                                             \
        FH = false

    // RRA
    case 0x1F:
        _RR(A);
        break;

    case 0xCB:
        ++PC;
        // _cycles += 1;

        switch (u8) {

        // RLC B
        case 0x00:
            _RLC(B);
            break;

        // RLC C
        case 0x01:
            _RLC(C);
            break;

        // RLC D
        case 0x02:
            _RLC(D);
            break;

        // RLC E
        case 0x03:
            _RLC(E);
            break;

        // RLC H
        case 0x04:
            _RLC(H);
            break;

        // RLC L
        case 0x05:
            _RLC(L);
            break;

        // RLC (HL)
        case 0x06:
            u8 = readByte(HL);
            _RLC(u8);
            writeByte(HL, u8);
            // _cycles += 2;
            break;

        // RLC A
        case 0x07:
            _RLC(A);
            break;

        // RRC B
        case 0x08:
            _RRC(B);
            break;

        // RRC C
        case 0x09:
            _RRC(C);
            break;

        // RRC D
        case 0x0A:
            _RRC(D);
            break;

        // RRC E
        case 0x0B:
            _RRC(E);
            break;

        // RRC H
        case 0x0C:
            _RRC(H);
            break;

        // RRC L
        case 0x0D:
            _RRC(L);
            break;

        // RRC (HL)
        case 0x0E:
            u8 = readByte(HL);
            _RRC(u8);
            writeByte(HL, u8);
            break;

        // RRC A
        case 0x0F:
            _RRC(A);
            break;

        // RL B
        case 0x10:
            _RL(B);
            break;

        // RL C
        case 0x11:
            _RL(C);
            break;

        // RL D
        case 0x12:
            _RL(D);
            break;

        // RL E
        case 0x13:
            _RL(E);
            break;

        // RL H
        case 0x14:
            _RL(H);
            break;

        // RL L
        case 0x15:
            _RL(L);
            break;

        // RL (HL)
        case 0x16:
            u8 = readByte(HL);
            _RL(u8);
            writeByte(HL, u8);
            // _cycles += 2;
            break;

        // RL A
        case 0x17:
            _RL(A);
            break;

        // RR B
        case 0x18:
            _RR(B);
            break;

        // RR C
        case 0x19:
            _RR(C);
            break;

        // RR D
        case 0x1A:
            _RR(D);
            break;

        // RR E
        case 0x1B:
            _RR(E);
            break;

        // RR H
        case 0x1C:
            _RR(H);
            break;

        // RR L
        case 0x1D:
            _RR(L);
            break;

        // R (HL)
        case 0x1E:
            u8 = readByte(HL);
            _RR(u8);
            writeByte(HL, u8);
            // _cycles += 2;
            break;

        // RR A
        case 0x1F:
            _RR(A);
            break;

        #define _SLA(X)                                                         \
            FC = ((X)&0x80) == 0x80;                                            \
            (X) <<= 1;                                                          \
            FZ = ((X) == 0);                                                    \
            FN = false;                                                         \
            FH = false

        // SLA B
        case 0x20:
            _SLA(B);
            break;

        // SLA C
        case 0x21:
            _SLA(C);
            break;

        // SLA D
        case 0x22:
            _SLA(D);
            break;

        // SLA E
        case 0x23:
            _SLA(E);
            break;

        // SLA H
        case 0x24:
            _SLA(H);
            break;

        // SLA L
        case 0x25:
            _SLA(L);
            break;

        // SLA (HL)
        case 0x26:
            u8 = readByte(HL);
            _SLA(u8);
            writeByte(HL, u8);
            // _cycles += 2;
            break;

        // SLA A
        case 0x27:
            _SLA(A);
            break;

        #define _SRA(X)                                                         \
            FC = ((X)&0x01) == 0x01;                                            \
            (X) = ((X) >> 1) | ((X)&0x80);                                      \
            FZ = ((X) == 0);                                                    \
            FN = false;                                                         \
            FH = false
        
        // SRA B
        case 0x28:
            _SRA(B);
            break;

        // SRA C
        case 0x29:
            _SRA(C);
            break;

        // SRA D
        case 0x2A:
            _SRA(D);
            break;

        // SRA E
        case 0x2B:
            _SRA(E);
            break;

        // SRA H
        case 0x2C:
            _SRA(H);
            break;

        // SRA L
        case 0x2D:
            _SRA(L);
            break;

        // SRA (HL)
        case 0x2E:
            u8 = readByte(HL);
            _SRA(u8);
            writeByte(HL, u8);
            // _cycles += 2;
            break;

        // SRA A
        case 0x2F:
            _SRA(A);
            break;

        #define _SRL(X)                                                         \
            FC = ((X)&0x01) == 0x01;                                            \
            (X) >>= 1;                                                          \
            FZ = ((X) == 0);                                                    \
            FN = false;                                                         \
            FH = false

        // SRL B
        case 0x38:
            _SRL(B);
            break;

        // SRL C
        case 0x39:
            _SRL(C);
            break;

        // SRL D
        case 0x3A:
            _SRL(D);
            break;

        // SRL E
        case 0x3B:
            _SRL(E);
            break;

        // SRL H
        case 0x3C:
            _SRL(H);
            break;

        // SRL L
        case 0x3D:
            _SRL(L);
            break;

        // SRL (HL)
        case 0x3E:
            u8 = readByte(HL);
            _SRL(u8);
            writeByte(HL, u8);
            // _cycles += 2;
            break;

        // SRL A
        case 0x3F:
            _SRL(A);
            break;

        #define _SWAP(X)                                                        \
            (X) = (((X)&0x0F) << 4) | (((X)&0xF0) >> 4);                        \
            FZ = ((X) == 0);                                                    \
            FN = false;                                                         \
            FH = false;                                                         \
            FC = false

        // SWAP B
        case 0x30:
            _SWAP(B);
            break;

        // SWAP C
        case 0x31:
            _SWAP(C);
            break;

        // SWAP D
        case 0x32:
            _SWAP(D);
            break;

        // SWAP E
        case 0x33:
            _SWAP(E);
            break;

        // SWAP H
        case 0x34:
            _SWAP(H);
            break;

        // SWAP L
        case 0x35:
            _SWAP(L);
            break;

        // SWAP (HL)
        case 0x36:
            u8 = readByte(HL);
            _SWAP(u8);
            writeByte(HL, u8);
            // _cycles += 2;
            break;

        // SWAP A
        case 0x37:
            _SWAP(A);
            break;

        #define _BIT(NUM, X)                                                    \
            FZ = (((X) & (1 << NUM)) == 0);                                  \
            FN = false;                                                      \
            FH = true

        #define _RES(NUM, X)                                                    \
            X &= ~(1 << NUM)

        #define _BIT_GROUP(NUM)                                                 \
            case (0x40 + ((NUM) * 8) + 0):                                      \
                _BIT((NUM), B);                                                 \
                break;                                                          \
            case (0x40 + ((NUM) * 8) + 1):                                      \
                _BIT((NUM), C);                                                 \
                break;                                                          \
            case (0x40 + ((NUM) * 8) + 2):                                      \
                _BIT((NUM), D);                                                 \
                break;                                                          \
            case (0x40 + ((NUM) * 8) + 3):                                      \
                _BIT((NUM), E);                                                 \
                break;                                                          \
            case (0x40 + ((NUM) * 8) + 4):                                      \
                _BIT((NUM), H);                                                 \
                break;                                                          \
            case (0x40 + ((NUM) * 8) + 5):                                      \
                _BIT((NUM), L);                                                 \
                break;                                                          \
            case (0x40 + ((NUM) * 8) + 6):                                      \
                _BIT((NUM), readByte(HL));                                      \
                /* _cycles += 1; */                                             \
                break;                                                          \
            case (0x40 + ((NUM) * 8) + 7):                                      \
                _BIT((NUM), A);                                                 \
                break;

        _BIT_GROUP(0)
        _BIT_GROUP(1)
        _BIT_GROUP(2)
        _BIT_GROUP(3)
        _BIT_GROUP(4)
        _BIT_GROUP(5)
        _BIT_GROUP(6)
        _BIT_GROUP(7)

        #define _RES_GROUP(NUM)                                                 \
            case (0x80 + ((NUM) * 8) + 0):                                      \
                _RES((NUM), B);                                                 \
                break;                                                          \
            case (0x80 + ((NUM) * 8) + 1):                                      \
                _RES((NUM), C);                                                 \
                break;                                                          \
            case (0x80 + ((NUM) * 8) + 2):                                      \
                _RES((NUM), D);                                                 \
                break;                                                          \
            case (0x80 + ((NUM) * 8) + 3):                                      \
                _RES((NUM), E);                                                 \
                break;                                                          \
            case (0x80 + ((NUM) * 8) + 4):                                      \
                _RES((NUM), H);                                                 \
                break;                                                          \
            case (0x80 + ((NUM) * 8) + 5):                                      \
                _RES((NUM), L);                                                 \
                break;                                                          \
            case (0x80 + ((NUM) * 8) + 6):                                      \
                u8 = readByte(HL);                                              \
                _RES((NUM), u8);                                                \
                writeByte(HL, u8);                                              \
                /* _cycles += 2; */                                             \
                break;                                                          \
            case (0x80 + ((NUM) * 8) + 7):                                      \
                _RES((NUM), A);                                                 \
                break;

        _RES_GROUP(0)
        _RES_GROUP(1)
        _RES_GROUP(2)
        _RES_GROUP(3)
        _RES_GROUP(4)
        _RES_GROUP(5)
        _RES_GROUP(6)
        _RES_GROUP(7)

        #define _SET(N, X)                                                      \
            X |= (1 << N)

        #define _SET_GROUP(NUM)                                                 \
            case (0xC0 + ((NUM) * 8) + 0):                                      \
                _SET((NUM), B);                                                 \
                break;                                                          \
            case (0xC0 + ((NUM) * 8) + 1):                                      \
                _SET((NUM), C);                                                 \
                break;                                                          \
            case (0xC0 + ((NUM) * 8) + 2):                                      \
                _SET((NUM), D);                                                 \
                break;                                                          \
            case (0xC0 + ((NUM) * 8) + 3):                                      \
                _SET((NUM), E);                                                 \
                break;                                                          \
            case (0xC0 + ((NUM) * 8) + 4):                                      \
                _SET((NUM), H);                                                 \
                break;                                                          \
            case (0xC0 + ((NUM) * 8) + 5):                                      \
                _SET((NUM), L);                                                 \
                break;                                                          \
            case (0xC0 + ((NUM) * 8) + 6):                                      \
                u8 = readByte(HL);                                              \
                _SET((NUM), u8);                                                \
                writeByte(HL, u8);                                              \
                /* _cycles += 2; */                                             \
                break;                                                          \
            case (0xC0 + ((NUM) * 8) + 7):                                      \
                _SET((NUM), A);                                                 \
                break;

        _SET_GROUP(0)
        _SET_GROUP(1)
        _SET_GROUP(2)
        _SET_GROUP(3)
        _SET_GROUP(4)
        _SET_GROUP(5)
        _SET_GROUP(6)
        _SET_GROUP(7)

        }

        break;
    default:
        // Error
        break;
    }

    if (willEnableIME) {
        IME = true;
        RequestIME = false;
    }

    checkInterrupts();
}

} // namespace DMG_CPU
} // namespace kiwi