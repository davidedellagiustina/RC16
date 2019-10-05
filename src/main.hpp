/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * MAIN HEADER
 * Della Giustina Davide
 * 05/10/2019
 */

// Libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
using namespace std;

// Macros
#define nl		'\n'
#define pb		bushback
#define iss		istringstream
#define oss		ostringstream
#define ifs		ifstream
#define ofs		ofstream

// Condition codes
#define AL      0x0
#define EQ      0x1
#define NE      0x2
#define LT      0x3
#define LE      0x4
#define GT      0x5
#define GE      0x6
#define VS      0x7
#define VC      0x8
#define CS      0x9
#define CC      0xa

// Registers
#define R0      0x0
#define R1      0x1
#define R2      0x2
#define R3      0x3
#define R4      0x4
#define R5      0x5
#define SP		0x5
#define R6      0x6
#define LR		0x6
#define R7      0x7
#define PC		0x7
#define A       0x8
#define B       0x9
#define OUT     0xa
#define MAR     0xb
#define OR      0xc

// ALU op-codes
#define ADD     0x0
#define AND     0x1
#define ORR     0x2
#define XOR     0x3
#define NOT     0x4
#define LSL     0x5
#define LSR     0x6
#define ASR     0x7

// Checks whether a file exists and is accessible or not.
// @param file		File name.
// @return			True if file exists and is accessible, false otherwise.
inline bool fexists(const string file) {
	ifs f(file);
	return (bool)f;
}

// Converts a binary 16-bit number to its hexadecimal string representation
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