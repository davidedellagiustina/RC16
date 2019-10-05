/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * ISA DEFINITION
 * Della Giustina Davide
 * 05/10/2019
 */

#ifndef ISA
#define ISA

// SET <reg> <val>: save immediate value to register.
// @param reg       Destination register.
// @param val       Immediate value.
// @param cond      Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string set(const int &reg, const int &val, const int &cond = AL) {
    oss os;
    try {
        os << SET(reg, val, cond);
    } catch (...) { throw "The given value is too big. Maximum is 63."; }
    return os.str();
}

// CMP <reg> <reg>: update flags basing on subtraction.
// @param reg1      Register 1.
// @param reg2      Register 2.
// @param cond      Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string cmp(const int &reg1, const int &reg2, const int &cond = AL) {
    oss os;
    // Flags do get updated, but result remains in OUT register
    os << MOVREG(reg1, A, cond);
    os << MOVREG(reg2, B, cond);
    os << EXC(SUB, true, true, cond);
    return os.str();
}

// JMP <rel_addr>: jump to a certain address.
// @param reladdr   Address to jump to. RELATIVE ADDRESS: this value is added to the current one (at the moment of this operation start). MAX: 63.
// @param cond      Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string jmp(const int &reladdr, const int &cond = AL) {
    oss os;
    // Since flags are not being updated, this version is fine
    os << MOVREG(PC, A, cond);
    try {
        os << SET(B, reladdr, cond);
    } catch (...) { throw "Cannot jump this long!"; }
    os << EXC(ADD, false, false, cond);
    os << MOVREG(OUT, PC, cond);
    return os.str();
}

// NOP: do nothing.
// @param cond      Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string nop(const int &cond = AL) {
    return NOP(cond);
}

// HLT: halt program execution.
// @param cond      Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string hlt(const int &cond = AL) {
    return HLT(cond);
}

#endif