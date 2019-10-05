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
inline string exc(const int cond = 0x0) {
    // TODO
}

// Implement NOP instruction.
// @param cond      Conditional.
// @return          Hexadecimal string representation of instruction.
inline string nop(const int cond = 0x0) {
    uint16_t instr = 0x0; // NOP = 0b00xxxx0000000000
    instr |= cond << 10; // Add condition
    return bin2hex(instr);
}

// Implement HLT instruction.
// @param cond      Conditional.
// @return          Hexadecimal string representation of instruction.
inline string hlt(const int cond = 0x0) {
    uint16_t instr = 0x1; // HLT = 0b00xxxx0000000001
    instr |= cond << 10; // Add condition
    return bin2hex(instr);
}

#endif