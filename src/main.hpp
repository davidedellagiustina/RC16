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
#include <unistd.h>
using namespace std;

// Macros
#define nl		'\n'
#define pb		push_back
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

// Memory sections
#define mem_init	0x0000
#define mem_end		0xffff // Total memory: 128kB
#define mem_idat	0x0008
#define mem_edat	0x3fff // Program data: ~32kB [0x0 -> 0x7 (16B) reserved for initialization]
#define mem_iprg	0x4000
#define mem_eprg	0xbfff // Program instructions: 64kB
#define mem_istk	0xc000
#define mem_estk	0xffff // Stack: 32kB

// Checks whether a file exists and is accessible or not.
// @param file		File name.
// @return			True if file exists and is accessible, false otherwise.
inline bool fexists(const string file) {
	ifs f(file);
	return (bool)f;
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

// Trim a string.
// @param s		String.
// @return		Trimmed string.
inline string trim(const string &s) {
	string t = s;
	while (t[0] == ' ') t = t.substr(1);
	while (t[t.length()-1] == ' ') t = t.substr(0, t.length()-1);
	return t;
}

// Transform a string to lowercase.
// @param s		String.
// @return		Lowercase string.
inline string lc(const string &s) {
	string t = "";
	for (char c : s) t += std::tolower(c);
	return t;
}

// Split a string basing on a certain delim character.
// @param s			String to be split.
// @param delim		Delimitator.
// @return			Vector of split strings.
inline vector<string> split(const string &s, const char &delim) {
	vector<string> out;
	int ptr = 0;
	if (s.length() > 0) out.pb("");
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] == delim && i < s.length()) {
			out.pb("");
			++ptr;
		} else if (s[i] != delim) out[ptr] += s[i];
	}
	return out;
}

// Convert string conditional to uint16_t.
// @param c		String conditional.
// @return		Integer conditional.
inline uint16_t condition(const string &c) {
	unordered_map<string,uint16_t> m;
	m["al"] = AL; m["eq"] = EQ; m["ne"] = NE; m["lt"] = LT; m["le"] = LE; m["gt"] = GT; m["ge"] = GE; m["vs"] = VS; m["vc"] = VC; m["cs"] = CS; m["cc"] = CC;
	return m[c];
}

// Compute number of microops needed to perform an instruction.
// @param instr		Line of code.
// @return			Number of microops.
inline int nom(const string &line, unordered_map<string,uint16_t> &lbl) {
	int c = 0;
	string instr = line.substr(0, 3);
	// Complex commands
	if (instr.compare("put") == 0) {
		vector<string> s = split(line, ' ');
		uint16_t addr = lbl[s[2].substr(1)], a1 = addr >> 12, a2 = (addr >> 6) & 0x3f, a3 = addr & 0x3f;
		if (a1 != 0x0) c += 3;
		if (a2 != 0x0) c += 3;
		if (a1 != 0x0 || a2 != 0x0) c += 3;
		if (a3 != 0x0) c += 2;
		c += 2;
	// Four/seven-microop commands
	} else if (instr.compare("add") == 0 || instr.compare("sub") == 0 || instr.compare("and") == 0 || instr.compare("orr") == 0 || instr.compare("eor") == 0 || instr.compare("lsl") == 0 || instr.compare("lsr") == 0 || instr.compare("asr") == 0) {
		bool s = false;
		string c = "al";
		if (line[3] == 's') s = true;
		if (s && line[4] != ' ') c = line.substr(4, 2);
		else if (!s && line[3] != ' ') c = line.substr(3, 2);
		uint16_t cond = condition(c);
		if (s && cond != AL) c += 3;
		c += 4;
	// Four-microop commands
	} else if (instr.compare("jmp") == 0) c += 4;
	// Three-microop commands
	else if (instr.compare("cmp") == 0) c += 3;
	// Two-microop commands
	else if (instr.compare("ldr") == 0 || instr.compare("str") == 0) c += 2;
	// One-microop commands
	else if (instr.compare("set") == 0 || instr.compare("mov") == 0 || instr.compare("prt") == 0 || instr.compare("nop") == 0 || instr.compare("hlt") == 0) ++c;
	// Unknown command
	else throw "Unknown instruction";
	return c;
}