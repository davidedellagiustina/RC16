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

// Convert string conditional to cond.
// @param c		String conditional.
// @return		Integer conditional.
inline cond condition(const string &c) {
	unordered_map<string,cond> m;
	m["al"] = AL; m["eq"] = EQ; m["ne"] = NE; m["lt"] = LT; m["le"] = LE; m["gt"] = GT; m["ge"] = GE; m["vs"] = VS; m["vc"] = VC; m["cs"] = CS; m["cc"] = CC;
	return m[c];
}

// Compute number of microops needed to perform an instruction.
// @param instr		Line of code.
// @return			Number of microops.
inline int nom(const string &line, unordered_map<string,uint16_t> &d_lbl) {
	int c = 0;
    vector<string> s = split(line, ':');
	string instr = s[0]; // Instruction
    vector<string> args;
    if (s.size() > 1) args = split(trim(s[1]), ','); // Arguments
	// Complex commands
	if (instr.compare("put") == 0) {
		uint16_t addr = d_lbl[trim(args[1]).substr(1)], a1 = addr >> 12, a2 = (addr >> 6) & 0x3f, a3 = addr & 0x3f;
		if (a1 != 0x0) c += 3;
		if (a2 != 0x0) c += 3;
		if (a1 != 0x0 || a2 != 0x0) c += 3;
		if (a3 != 0x0) c += 2;
		c += 2;
	// Four/seven-microop commands
	} else if (instr.compare("add") == 0 || instr.compare("sub") == 0 || instr.compare("and") == 0 || instr.compare("orr") == 0 || instr.compare("eor") == 0 || instr.compare("lsl") == 0 || instr.compare("lsr") == 0 || instr.compare("asr") == 0) {
		bool s = false;
		string _c = "al";
		if (line[3] == 's') s = true;
		if (s && line[4] != ' ') _c = line.substr(4, 2);
		else if (!s && line[3] != ' ') _c = line.substr(3, 2);
		cond cnd = condition(_c);
		if (s && cnd != AL) c += 3;
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
	else throw invalid_argument("Unknown instruction");
	return c;
}

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
    // 1) Parse .data section & compute pointers addresses
    ifs prg(src);
    uint16_t ptr = 0x0000, eod = 0x0008;
    int sec = 0; // Program section: 0 -> none, 1 -> data, 2 -> prgm
    int c = 0; // Line counter
    unordered_map<string,uint16_t> d_lbl; // Labels addresses (in .data section)
    unordered_map<string,uint16_t> p_lbl; // Labels addresses (in .prgm section)
    string line;
    while (getline(prg, line)) {
        ++c;
        line = lc(trim(line)); // Trim and lowercase
        if (line.compare("") == 0 || line[0] == '#') continue; // Skip empty lines and comments
        if (line.compare(".data") == 0) { sec = 1; ptr = 0x0008; continue; } // Start of data section
        else if (line.compare(".prgm") == 0) { sec = 2; eod = ptr; ptr = 0x4000; continue; } // Start of prgm section
        if (sec == 1) { // Write data
            vector<string> values;
            if (line[0] == '&') { // If there is a label on this line
                vector<string> s = split(line, '=');
                d_lbl[s[0].substr(1)] = ptr; // Store label address
                values = split(trim(s[1]), ',');
            } else values = split(line, ',');
            for (string q : values) { // Values
                bin << bin2hex(stoi(trim(q), nullptr, 10)) << " ";
                ++ptr;
            }
        } else if (sec == 2) {
            if (line[0] == '&') p_lbl[line.substr(1)] = ptr; // Store label address
            else {
                try {
                    ptr += nom(line, d_lbl);
                } catch (invalid_argument &e) {
                    cerr << "Error on line " << c << nl;
                }
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
            if (line[0] == '&') continue; // Skip labels
            else bin << parseLine(line); // Parse single line
        }
    }
    prg.close();
    prg.close();
    return bin.str();
}

#endif