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
#define minval          0x0 // Minimum immediate value
#define maxval          0x3f // Max immediate value
#define minval_err      out_of_range("The given immediate value is negative.")
#define maxval_err      out_of_range("The given immediate value exceeds 6 bits.")
#define ireg(r)         ((r>=R0 && r<=R7) || r==A || r==B || r==MAR || r==OR) // Valid input registers
#define oreg(r)         ((r>=R0 && r<=R7) || r==OUT) // Valid output registers
#define ireg_err        invalid_argument("The given register is not a valid input register.")
#define oreg_err        invalid_argument("The given register is not a valid output register.")

// Implement MOV (reg->reg) instruction.
// @param r1        Source register [4 bits].
// @param r2        Destination register [4 bits].
// @param c         Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string MOVREG(const reg &r1, const reg &r2, const cond &c = AL) {
    uint16_t instr = 0x4000; // MOV(reg->reg) = 0b01.xxxx.0.xxxx.xxxx.0
    instr |= c << 10; // Add conditional
    if (oreg(r1)) instr |= r1 << 5; // Add source register
    if (ireg(r2)) instr |= r2 << 1; // Add destination register
    return bin2hex(instr);
}

// Implement MOV (mem-op) instruction.
// @param w         Write if set, read otherwise.
// @param r         Source/destination register (depending on w) [4 bits].
// @param c         Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string MOVMEM(const bool &w, const reg &r, const cond &c = AL) {
    uint16_t instr = 0x4200; // MOV (mem-op) = 0b01.xxxx.1.x.xxxx.0000
    instr |= c << 10; // Add conditional
    if (w) instr |= 1 << 8; // Add 'r/w' flag
    if ((w && oreg(r)) || (!w && ireg(r))) instr |= r << 4; // Add source/destination register
    else if (w) throw oreg_err;
    else throw ireg_err;
    return bin2hex(instr);
}

// Implement SET instruction.
// @param r         Destination register [4 bits].
// @param val       Immediate value [6 bits].
// @param c         Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string SET(const reg &r, const int &val, const cond &c = AL) {
    uint16_t instr = 0x8000; // SET = 0b01.xxxx.xxxx.xxxxxx
    instr |= c << 10; // Add conditional
    if (ireg(r)) instr |= r << 6; // Add destination register
    else throw ireg_err;
    if (val >= minval && val <= maxval) instr |= val; // Add immediate value
    else if (val >= minval) throw maxval_err;
    else throw minval_err;
    return bin2hex(instr);
}

// Implement EXC instruction.
// @param opcode    ALU operation code [3 bits].
// @param notb      ALU '~B' flag [1 bit].
// @param setflags  ALU 'set flags' flag [1 bit].
// @param c         Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string EXC(const alu_op &opcode, const bool &notb, const bool &setflags, const cond &c = AL) {
    uint16_t instr = 0xc000; // EXC = 0b11.xxxx.xxx.x.x.00000
    instr |= c << 10; // Add conditional
    instr |= opcode << 7; // Add ALU op-code
    if (notb) instr |= 1 << 6; // Add '~B' flag
    if (setflags) instr |= 1 << 5; // Add 'set flags' flag
    return bin2hex(instr);
}

// Implement NOP instruction.
// @param c         Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string NOP(const cond &c = AL) {
    uint16_t instr = 0x0; // NOP = 0b00.xxxx.000000000.0
    instr |= c << 10; // Add conditional
    return bin2hex(instr);
}

// Implement HLT instruction.
// @param c         Conditional [4 bits]. Default: AL.
// @return          Hexadecimal string representation of instruction.
inline string HLT(const cond &c = AL) {
    uint16_t instr = 0x1; // HLT = 0b00.xxxx.000000000.1
    instr |= c << 10; // Add conditional
    return bin2hex(instr);
}

#endif