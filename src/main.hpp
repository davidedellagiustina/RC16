/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * MAIN HEADER
 * Davide Della Giustina
 * 05/10/2019
 */

// Libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <unistd.h>
using namespace std;

// Macros
#define pb		push_back

// Types
typedef istringstream iss;
typedef ostringstream oss;
typedef ifstream ifs;
typedef ofstream ofs;

// Constexprs
constexpr char nl = '\n';
constexpr uint16_t mem_init = 0x0000;
constexpr uint16_t mem_end = 0xffff; // Total memory: 128kB
constexpr uint16_t mem_idat = 0x0008;
constexpr uint16_t mem_edat = 0x3fff; // Program data: ~32kB [0x0 -> 0x7 (16B) reserved for initialization]
constexpr uint16_t mem_iprg = 0x4000;
constexpr uint16_t mem_eprg = 0x7fff; // Program instructions: 32kB
constexpr uint16_t mem_istk = 0x8000;
constexpr uint16_t mem_estk = 0xffff; // Stack: 64kB

// Condition codes
enum cond : uint8_t { AL = 0x0, EQ = 0x1, NE = 0x2, LT = 0x3, LE = 0x4, GT = 0x5, GE = 0x6, VS = 0x7, VC = 0x8, CS = 0x9, CC = 0xa };

// Registers
enum reg : uint8_t { R0 = 0x0, R1 = 0x1, R2 = 0x2, R3 = 0x3, R4 = 0x4, R5 = 0x5, SP = 0x5, R6 = 0x6, LR = 0x6, R7 = 0x7, PC = 0x7, A = 0x8, B = 0x9, OUT = 0xa, MAR = 0xb, OR = 0xc, JR = 0xd };

// ALU op-codes
enum alu_op : uint8_t { ADD = 0x0, AND = 0x1, ORR = 0x2, EOR = 0x3, NOT = 0x4, LSL = 0x5, LSR = 0x6, ASR = 0x7 };

// Checks whether a file exists and is accessible or not.
// @param file		File name.
// @return			True if file exists and is accessible, false otherwise.
inline bool fexists(const string &file) {
	ifs f(file);
	bool r = (bool)f;
	f.close();
	return r;
}

// Converts a binary 16-bit number to its hexadecimal string representation.
// @param bin		Binary number.
// @return			Hexadecimal string representation of the number.
inline string bin2hex(const uint16_t bin) {
	string hex = "";
	char hexmap[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	int shift = 12;
	for (uint16_t bitmask = 0xf000; bitmask != 0x0; bitmask >>= 4) {
		hex += hexmap[(bin&bitmask)>>shift];
		shift -= 4;
	}
	return hex;
}