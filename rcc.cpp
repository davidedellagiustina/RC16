/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * MAIN
 * Davide Della Giustina
 * 05/10/2019
 */

#include "src/main.hpp"
#include "src/microops.cpp"
#include "src/isa.cpp"
#include "src/parser.cpp"

// Prints usage help.
// @return		String with usage help.
string help() {
	oss os;
	os << "Usage: rcc [options]" << nl <<
	"Options:" << nl <<
	" -i <arg>	Input file to be compiled [REQUIRED]." << nl <<
	" -o <arg>	Output file name." << nl <<
	" -h		Print this help.";
	return os.str();
}

// Compile a program into a binary file.
// @param src		Source filename.
// @param dst		Destination filename.
void compilePrg(const string &src, const string &dst) {
	ofs bin(dst);
	// Header
	bin << "v2.0 raw" << nl;
	// Init instructions
	bin << SET(A, 0) << " " << EXC(NOT, false, false) << " " << MOVREG(OUT, SP) << nl; // SP = mem_estk (0xffff)
	bin << SET(A, 32) << " " << SET(B, 9) << " " << EXC(LSL, false, false) << " " << MOVREG(OUT, LR) << nl; // LR = mem_iprg (0x4000)
	bin << MOVREG(OUT, PC) << nl; // PC = mem_iprg (0x4000)
	// Parse program
	bin << parsePrg(src);
	bin.close();
}

// Global variables
unordered_map<string,uint16_t> labels;

// Main.
int main(int argc, char* argv[]) {
	string ifile, ofile = "";
	// Parse command line options
	int opt;
	while ((opt = getopt(argc, argv, "i:o:h")) != -1) {
		switch (opt) {
			case 'i':
				ifile = string(optarg);
				break;
			case 'o':
				ofile = string(optarg);
				break;
			case 'h':
				cout << help() << nl;
				return 0;
			default:
				cerr << help() << nl;
				return -1;
		}
	}
	if (ifile.compare("") == 0) { cerr << "No input file given." << nl; return -1; }
	if (!fexists(ifile)) { cerr << "Given file does not exist or is unaccessible." << nl; return -1; } // Check ifile existence and accessibility
	if (ofile.compare("") == 0) ofile = "a.bin"; // If ofile not given
	// Compile given program
	compilePrg(ifile, ofile);
	return 0;
}