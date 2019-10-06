/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * MICRO OPERATIONS DEFINITION
 * Davide Della Giustina
 * 05/10/2019
 */

#ifndef MOPS
#define MOPS

// Bit length and correctness checks
#define maxreg          0xc // Max register address
#define maxcond         0xa // Max conditional code
#define maxop           0x7 // Max ALU op-code
#define minval          0x0 // Minimum immediate value
#define maxval          0x3f // Max immediate value
#define maxreg_err      "The given register code exceeds 4 bits or does not exist."
#define maxcond_err     "The given conditional code exceeds 4 bits or does not exist."
#define maxop_err       "The given ALU op-code exceeds 3 bits."
#define minval_err      "The given immediate value is negative."
#define maxval_err      "The given immediate value exceeds 6 bits."
#define ireg(r)         ((r>=R0 && r<=R7) || r==A || r==B || r==MAR || r==OR) // Valid input registers
#define oreg(r)         ((r>=R0 && r<=R7) || r==OUT) // Valid output registers
#define ireg_err        "The given register is not a valid input register."
#define oreg_err        "The given register is not a valid output register."

// Implement MOV (reg->reg) instruction.
// @param reg1      Source register [4 bits].
// @param reg2      Destination register [4 bits].
// @param cond      Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string MOVREG(const int &reg1, const int &reg2, const int &cond = AL) {
    uint16_t instr = 0x4000; // MOV(reg->reg) = 0b01.xxxx.0.xxxx.xxxx.0
    if (cond <= maxcond) instr |= cond << 10; // Add conditional
    else throw maxcond_err;
    if (reg1 <= maxreg && reg2 <= maxreg) {
        if (oreg(reg1)) instr |= reg1 << 5; // Add source register
        else throw oreg_err;
        if (ireg(reg2)) instr |= reg2 << 1; // Add destination register
        else throw ireg_err;
    } else throw maxreg_err;
    return bin2hex(instr);
}

// Implement MOV (mem-op) instruction.
// @param w         Write if set, read otherwise.
// @param reg       Source/destination register (depending on w) [4 bits].
// @param cond      Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string MOVMEM(const bool &w, const int &reg, const int &cond = AL) {
    uint16_t instr = 0x4200; // MOV (mem-op) = 0b01.xxxx.1.x.xxxx.0000
    if (cond <= maxcond) instr |= cond << 10; // Add conditional
    else throw maxcond_err;
    if (w) instr |= 1 << 8; // Add 'r/w' flag
    if (reg <= maxreg) {
        if ((w && oreg(reg)) || (!w && ireg(reg))) instr |= reg << 4; // Add source/destination register
        else if (w) throw oreg_err;
        else throw ireg_err;
    } else throw maxreg_err;
    return bin2hex(instr);
}

// Implement SET instruction.
// @param reg       Destination register [4 bits].
// @param val       Immediate value [6 bits].
// @param cond      Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string SET(const int &reg, const int &val, const int &cond = AL) {
    uint16_t instr = 0x8000; // SET = 0b01.xxxx.xxxx.xxxxxx
    if (cond <= maxcond) instr |= cond << 10; // Add conditional
    else throw maxcond_err;
    if (reg <= maxreg) {
        if (ireg(reg)) instr |= reg << 6; // Add destination register
        else throw ireg_err;
    } else throw maxreg_err;
    if (val >= minval && val <= maxval) instr |= val; // Add immediate value
    else if (val >= minval) throw maxval_err;
    else throw minval_err;
    return bin2hex(instr);
}

// Implement EXC instruction.
// @param opcode    ALU operation code [3 bits].
// @param notb      ALU '~B' flag [1 bit].
// @param setflags  ALU 'set flags' flag [1 bit].
// @param cond      Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string EXC(const int &opcode, const bool &notb, const bool &setflags, const int &cond = AL) {
    uint16_t instr = 0xc000; // EXC = 0b11.xxxx.xxx.x.x.00000
    if (cond <= maxcond) instr |= cond << 10; // Add conditional
    else throw maxcond_err;
    if (opcode <= maxop) instr |= opcode << 7; // Add ALU op-code
    else throw maxop_err;
    if (notb) instr |= 1 << 6; // Add '~B' flag
    if (setflags) instr |= 1 << 5; // Add 'set flags' flag
    return bin2hex(instr);
}

// Implement NOP instruction.
// @param cond      Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string NOP(const int &cond = AL) {
    uint16_t instr = 0x0; // NOP = 0b00.xxxx.000000000.0
    if (cond <= maxcond) instr |= cond << 10; // Add conditional
    else throw maxcond_err;
    return bin2hex(instr);
}

// Implement HLT instruction.
// @param cond      Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string HLT(const int &cond = AL) {
    uint16_t instr = 0x1; // HLT = 0b00.xxxx.000000000.1
    if (cond <= maxcond) instr |= cond << 10; // Add conditional
    else throw maxcond_err;
    return bin2hex(instr);
}

#endif