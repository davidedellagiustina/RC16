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
#define nl		"\n"
#define pb		bushback
#define iss		istringstream
#define oss		ostringstream
#define ifs		ifstream
#define ofs		ofstream

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