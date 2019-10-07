/**
 * ===================
 * RCC - RC16 COMPILER
 * ===================
 * 
 * PROGRAM PARSER
 * Davide Della Giustina
 * 06/10/2019
 */

#ifndef PAR
#define PAR

// Parse a single line of code (a single command)
// @param line      Line of code.
// @return          Corresponding command.
inline string parseLine(const string &line) {
    oss os;
    // TODO
    return os.str();
}

// Parse a text file and build the binary code.
// @param src       Name of the source file.
// @return          String containing the assembled code.
string parsePrg(const string &src) {
    oss bin;
    // 1) Write data section and compute pointers addresses
    ifs prg(src);
    uint16_t ptr = 0x0, eod = 0x8;
    int sec = 0; // Program section: 0 -> none, 1 -> data, 2 -> prgm
    int c = 0; // Line counter
    unordered_map<string,uint16_t> lbl; // Labels addresses
    string line;
    while (getline(prg, line)) {
        ++c;
        line = lc(trim(line)); // Trim and lowercase
        if (line.compare("") == 0 || line[0] == '#') continue; // Skip empty lines and comments
        if (line.compare(".data") == 0) { sec = 1; ptr = 0x8; continue; } // Start of data section
        else if (line.compare(".prgm") == 0) { sec = 2; eod = ptr; ptr = 0x4000; continue; } // Start of prgm section
        if (sec == 1) { // Write data
            vector<string> s = split(line, ' ');
            lbl[s[0].substr(1)] = ptr; // Store label address
            for (int i = 1; i < s.size(); ++i) {
                bin << bin2hex(stoi(s[i], nullptr, 10)) << " ";
                ++ptr;
            }
        } else if (sec == 2) {
            if (line[0] == ':') lbl[line.substr(1)] = ptr; // Store label address
            else {
                try {
                    ptr += nom(line, lbl);
                } catch (...) { cerr << "Error on line " << c << nl; }
            }
        }
    }
    bin << nl << (mem_iprg - eod) << "*0" << nl; // Fill remaining data section with 0s
    prg.close();
    // 2) Parse .prgm section
    prg.open(src);
    c = 0; sec = 0;
    while (getline(prg, line)) {
        ++c;
        line = lc(trim(line));
        if (line.compare("") == 0 || line[0] == '#') continue;
        if (line.compare(".prgm") == 0) { sec = 2; continue; }
        if (sec == 2) {
            if (line[0] == ':') continue;
            else bin << parseLine(line); // Parse single line
        }
    }
    prg.close();
    prg.close();
    return bin.str();
}

#endif