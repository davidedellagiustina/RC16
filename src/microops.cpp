/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * MICRO OPERATIONS DEFINITION
 * Della Giustina Davide
 * 05/10/2019
 */

#ifndef MOPS
#define MOPS

// Bit length checks
#define maxreg          0xf
#define maxcond         0xf
#define maxop           0x7
#define maxreg_err      "The given register code exceeds 4 bits."
#define maxcond_err     "The given conditional code exceeds 4 bits."
#define maxop_err       "The given ALU op-code exceeds 3 bits."

// Implement MOV instruction.
// @param cond      Conditional.
// @return          Hexadecimal string representation of instruction.
inline string mov(const int cond = 0x0) {
    // TODO
}

// Implement SET instruction.
// @param cond      Conditional.
// @return          Hexadecimal string representation of instruction.
inline string set(const int cond = 0x0) {
    // TODO
}

// Implement EXC instruction.
// @param cond      Conditional.
// @return          Hexadecimal string representation of instruction.
inline string exc(const int &opcode, const bool &notb, const bool &setflags, const int &cond = 0x0) {
    uint16_t instr = 0xc000; // EXC = 0b11.xxxx.xxx.x.x.00000
    if (cond <= maxcond) instr |= cond << 10; // Add condition
    else throw maxcond_err;
    if (opcode <= maxop) instr |= opcode << 7; // Add ALU op-code
    else throw maxop_err;
    if (notb) instr |= 1 << 6; // Add '~B' flag
    if (setflags) instr |= 1 << 5; // Add 'set flags' flag
    return bin2hex(instr);
}

// Implement NOP instruction.
// @param cond      Conditional.
// @return          Hexadecimal string representation of instruction.
inline string nop(const int &cond = 0x0) {
    uint16_t instr = 0x0; // NOP = 0b00.xxxx.000000000.0
    if (cond <= maxcond) instr |= cond << 10; // Add condition
    else throw maxcond_err;
    return bin2hex(instr);
}

// Implement HLT instruction.
// @param cond      Conditional.
// @return          Hexadecimal string representation of instruction.
inline string hlt(const int &cond = 0x0) {
    uint16_t instr = 0x1; // HLT = 0b00.xxxx.000000000.1
    if (cond <= maxcond) instr |= cond << 10; // Add condition
    else throw maxcond_err;
    return bin2hex(instr);
}

#endif