/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * ISA DEFINITION
 * Davide Della Giustina
 * 05/10/2019
 */

#ifndef ISA
#define ISA

// PUT <reg> <addr>: load a register with a 16bit address. Very expensive.
// @param r         Destination register
// @param addr      Address.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string put(const reg &r, const uint16_t &addr, const cond &c = AL) {
    oss os;
    uint16_t a1 = addr >> 12; // Part 1 [4 bits]
    uint16_t a2 = (addr >> 6) & 0x3f; // Part 2 [6 bits]
    uint16_t a3 = addr & 0x3f; // Part 3 [6 bits]
    os << SET(A, a1, c) << " ";
    if (a1 != 0x0) os << SET(B, 6, c) << " " << EXC(LSL, false, false, c) << " " << MOVREG(OUT, A, c) << " ";
    if (a2 != 0x0) os << SET(B, a2, c) << " " << EXC(ORR, false, false, c) << " " << MOVREG(OUT, A, c) << " ";
    if (a1 != 0x0 || a2 != 0x0) os << SET(B, 6, c) << " " << EXC(LSL, false, false, c) << " " << MOVREG(OUT, A, c) << " ";
    if (a3 != 0x0) os << SET(B, a3, c) << " " << EXC(ORR, false, false, c) << " ";
    os << MOVREG(OUT, r, c) << " ";
    return os.str();
}

// SET <reg> <val>: save immediate value to register.
// @param r         Destination register.
// @param val       Immediate value.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string set(const reg &r, const int &val, const cond &c = AL) {
    oss os;
    try {
        os << SET(r, val, c) << " ";
    } catch (out_of_range &e) {
        throw e;
    } catch (invalid_argument &e) {
        throw e;
    }
    return os.str();
}

// MOV <reg> <reg>: move a value from a register to another.
// @param r1        Source register.
// @param r2        Destination register.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string mov(const reg &r1, const reg &r2, const cond &c = AL) {
    return MOVREG(r1, r2, c) + " ";
}

// LDR <reg> <reg_addr>: load value from memory to register.
// @param r         Destination register.
// @param r_addr    Register containing memory address.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string ldr(const reg &r, const reg &r_addr, const cond &c = AL) {
    oss os;
    os << MOVREG(r_addr, MAR, c) << " ";
    try {
        os << MOVMEM(false, r, c) << " ";
    } catch (invalid_argument &e) {
        throw e;
    }
    return os.str();
}

// STR <reg> <reg_addr>: store value in memory from register.
// @param r         Source register.
// @param r_addr    Register containing memory address.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string str(const reg &r, const reg &r_addr, const cond &c = AL) {
    oss os;
    os << MOVREG(r_addr, MAR, c) << " ";
    try {
        os << MOVMEM(true, r, c) << " ";
    } catch (invalid_argument &e) {
        throw e;
    }
    return os.str();
}

// ADD <reg> <reg> <reg>: add two values into a register.
// @param r1        Register containing operand 1.
// @param r2        Register containing operand 2.
// @param r3        Destination register.
// @param s         Update flags if 1, do not otherwise.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string add(const reg &r1, const reg &r2, const reg &r3, const bool &s, const cond &c = AL) {
    oss os;
    if (s && c != AL) { // If flags need to get updated and condition is not AL, we need to copy r3 to OUT to avoid errors
        os << MOVREG(r3, A) << " ";
        os << SET(B, 0) << " ";
        os << EXC(ADD, false, false) << " ";
    }
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r2, B, c) << " ";
    os << EXC(ADD, false, s, c) << " ";
    os << MOVREG(OUT, r3, (s?AL:c)) << " ";
    return os.str();
}

// SUB <reg> <reg> <reg>: subtract two values into a register.
// @param r1        Register containing operand 1.
// @param r2        Register containing operand 2.
// @param r3        Destination register.
// @param s         Update flags if 1, do not otherwise.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string sub(const reg &r1, const reg &r2, const reg &r3, const bool &s, const cond &c = AL) {
    oss os;
    if (s && c != AL) { // If flags need to get updated and condition is not AL, we need to copy r3 to OUT to avoid errors
        os << MOVREG(r3, A) << " ";
        os << SET(B, 0) << " ";
        os << EXC(ADD, false, false) << " ";
    }
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r2, B, c) << " ";
    os << EXC(ADD, true, s, c) << " ";
    os << MOVREG(OUT, r3, (s?AL:c)) << " ";
    return os.str();
}

// AND <reg> <reg> <reg>: perform bitwise and of two values into a register.
// @param r1        Register containing operand 1.
// @param r2        Register containing operand 2.
// @param r3        Destination register.
// @param s         Update flags if 1, do not otherwise.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string _and(const reg &r1, const reg &r2, const reg &r3, const bool &s, const cond &c = AL) {
    oss os;
    if (s && c != AL) { // If flags need to get updated and condition is not AL, we need to copy r3 to OUT to avoid errors
        os << MOVREG(r3, A) << " ";
        os << SET(B, 0) << " ";
        os << EXC(ADD, false, false) << " ";
    }
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r2, B, c) << " ";
    os << EXC(AND, false, s, c) << " ";
    os << MOVREG(OUT, r3, (s?AL:c)) << " ";
    return os.str();
}

// ORR <reg> <reg> <reg>: perform bitwise or of two values into a register.
// @param r1        Register containing operand 1.
// @param r2        Register containing operand 2.
// @param r3        Destination register.
// @param s         Update flags if 1, do not otherwise.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string orr(const reg &r1, const reg &r2, const reg &r3, const bool &s, const cond &c = AL) {
    oss os;
    if (s && c != AL) { // If flags need to get updated and condition is not AL, we need to copy r3 to OUT to avoid errors
        os << MOVREG(r3, A) << " ";
        os << SET(B, 0) << " ";
        os << EXC(ADD, false, false) << " ";
    }
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r2, B, c) << " ";
    os << EXC(ORR, false, s, c) << " ";
    os << MOVREG(OUT, r3, (s?AL:c)) << " ";
    return os.str();
}

// EOR <reg> <reg> <reg>: perform bitwise xor of two values into a register.
// @param r1        Register containing operand 1.
// @param r2        Register containing operand 2.
// @param r3        Destination register.
// @param s         Update flags if 1, do not otherwise.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string eor(const reg &r1, const reg &r2, const reg &r3, const bool &s, const cond &c = AL) {
    oss os;
    if (s && c != AL) { // If flags need to get updated and condition is not AL, we need to copy r3 to OUT to avoid errors
        os << MOVREG(r3, A) << " ";
        os << SET(B, 0) << " ";
        os << EXC(ADD, false, false) << " ";
    }
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r2, B, c) << " ";
    os << EXC(EOR, false, s, c) << " ";
    os << MOVREG(OUT, r3, (s?AL:c)) << " ";
    return os.str();
}

// LSL <reg> <reg> <reg_shift>: perform logical left shift of a register into another register.
// @param r1        Register containing operand 1.
// @param r_shift   Register containing shift value.
// @param r2        Destination register.
// @param s         Update flags if 1, do not otherwise.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string lsl(const reg &r1, const reg &r_shift, const reg &r2, const bool &s, const cond &c = AL) {
    oss os;
    if (s && c != AL) { // If flags need to get updated and condition is not AL, we need to copy reg2 to OUT to avoid errors
        os << MOVREG(r2, A) << " ";
        os << SET(B, 0) << " ";
        os << EXC(ADD, false, false) << " ";
    }
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r_shift, B, c) << " ";
    os << EXC(LSL, false, s, c) << " ";
    os << MOVREG(OUT, r2, (s?AL:c)) << " ";
    return os.str();
}

// LSR <reg> <reg> <reg_shift>: perform logical right shift of a register into another register.
// @param r1        Register containing operand 1.
// @param r_shift   Register containing shift value.
// @param r2        Destination register.
// @param s         Update flags if 1, do not otherwise.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string lsr(const reg &r1, const reg &r_shift, const reg &r2, const bool &s, const cond &c = AL) {
    oss os;
    if (s && c != AL) { // If flags need to get updated and condition is not AL, we need to copy reg2 to OUT to avoid errors
        os << MOVREG(r2, A) << " ";
        os << SET(B, 0) << " ";
        os << EXC(ADD, false, false) << " ";
    }
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r_shift, B, c) << " ";
    os << EXC(LSR, false, s, c) << " ";
    os << MOVREG(OUT, r2, (s?AL:c)) << " ";
    return os.str();
}

// ASR <reg> <reg> <reg_shift>: perform arithmetical right shift of a register into another register.
// @param r1        Register containing operand 1.
// @param r_shift   Register containing shift value.
// @param r2        Destination register.
// @param s         Update flags if 1, do not otherwise.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string asr(const reg &r1, const reg &r_shift, const reg &r2, const bool &s, const cond &c = AL) {
    oss os;
    if (s && c != AL) { // If flags need to get updated and condition is not AL, we need to copy reg2 to OUT to avoid errors
        os << MOVREG(r2, A) << " ";
        os << SET(B, 0) << " ";
        os << EXC(ADD, false, false) << " ";
    }
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r_shift, B, c) << " ";
    os << EXC(ASR, false, s, c) << " ";
    os << MOVREG(OUT, r2, (s?AL:c)) << " ";
    return os.str();
}

// PRT <reg>: print value contained in register.
// @param r         Source register.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string prt(const reg &r, const cond &c = AL) {
    return MOVREG(r, OR, c) + " ";
}

// CMP <reg> <reg>: update flags basing on subtraction.
// @param r1        Register 1.
// @param r2        Register 2.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string cmp(const reg &r1, const reg &r2, const cond &c = AL) {
    oss os;
    // Flags do get updated, but result remains in OUT register
    os << MOVREG(r1, A, c) << " ";
    os << MOVREG(r2, B, c) << " ";
    os << EXC(ADD, true, true, c) << " ";
    return os.str();
}

// JMP <addr>: jump to a certain address.
// @param addr      Address to jump to. OFFSET: this value is added to the start of the code segment. MAX: 0x3fff (32kB code segment covered).
// @param c         Conditional. Default: AL.
// @return          Hexadecimal string representation of command.
inline string jmp(const uint16_t &addr, const cond &c = AL) {
    oss os;
    try {
        os << LJR(addr) << " "; // Load JR
    } catch (out_of_range &e) {
        throw e;
    }
    os << MOVREG(JR, PC, c) << " "; // Move to PC (with condition)
    return os.str();
}

// PSH <reg>: push a register to the stack.
// @param r         Register to be pushed on the stack.
// @param c         Conditional. Default: AL.
// @return          Hexadecimal string representation of command.
inline string psh(const reg &r, const cond &c = AL) {
    oss os;
    // Push register
    os << MOVREG(LR, MAR, c) << " ";
    try {
        os << MOVMEM(true, r, c) << " ";
    } catch (invalid_argument &e) {
        throw e;
    }
    // Decrement link register
    os << MOVREG(LR, A, c) << " ";
    os << SET(B, 1, c) << " ";
    os << EXC(ADD, true, false, c) << " ";
    os << MOVREG(OUT, LR, c) << " ";
    return os.str();
}

// POP <reg>: pop a register from the stack.
// @param r         Register to be popped from the stack.
// @param c         Conditional. Default: AL.
// @return          Hexadecimal string representation of command.
inline string pop(const reg &r, const cond &c = AL) {
    oss os;
    // Increment link register
    os << MOVREG(LR, A, c) << " ";
    os << SET(B, 1, c) << " ";
    os << EXC(ADD, false, false, c) << " ";
    os << MOVREG(OUT, LR, c) << " ";
    // Pop register
    os << MOVREG(LR, MAR, c) << " ";
    try {
        os << MOVMEM(false, r, c) << " ";
    } catch (invalid_argument &e) {
        throw e;
    }
    return os.str();
}

// CAL <addr>: call a function (label or address).
// @param addr      Address to jump to.
// @param c         Conditional. Default: AL.
// @return          Hexadecimal string representation of command.
inline string cal(const uint16_t &addr, const cond &c = AL) {
    oss os;
    // Compute address of next instruction (not PC points to next microinstruction)
    os << MOVREG(PC, A, c) << " ";
    os << SET(B, 5, c) << " ";
    os << EXC(ADD, false, false, c) << " ";
    os << MOVREG(OUT, LR, c) << " ";
    // Jump
    try {
        os << LJR(addr) << " ";
    } catch (invalid_argument &e) {
        throw e;
    }
    os << MOVREG(JR, PC, c) << " ";
    return os.str();
}

// RET: return from a function.
// @param c         Conditional. Default: AL.
// @return          Hexadecimal string representation of command.
inline string ret(const cond &c = AL) {
    oss os;
    os << MOVREG(LR, PC, c) << " ";
    return os.str();
}

// NOP: do nothing.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string nop(const cond &c = AL) {
    return NOP(c) + " ";
}

// HLT: halt program execution.
// @param c         Conditional. Defualt: AL.
// @return          Hexadecimal string representation of command.
inline string hlt(const cond &c = AL) {
    return HLT(c) + " ";
}

#endif