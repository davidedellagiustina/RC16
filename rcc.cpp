/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * MAIN
 * Della Giustina Davide
 * 05/10/2019
 */

#include "src/main.hpp"
#include "src/microops.cpp"

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
	if (ofile.compare("") == 0) ofile = "a"; // If ofile not given
	ofile += ".rc"; // Extension
	// Compile given file
	// TODO
	return 0;
}