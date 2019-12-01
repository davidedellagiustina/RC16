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

// Convert string register to reg.
// @param r     String register.
// @return      Integer register.
inline reg regst(const string &r) {
    unordered_map<string,reg> m;
    m["r0"] = R0; m["r1"] = R1; m["r2"] = R2; m["r3"] = R3; m["r4"] = R4; m["r5"] = R5; m["sp"] = SP; m["r6"] = R6; m["lr"] = LR; m["r7"] = R7; m["pc"] = PC;
    m["a"] = A; m["b"] = B; m["out"] = OUT; m["mar"] = MAR; m["or"] = OR;
    return m[r];
}

// Compute number of microops needed to perform an instruction.
// @param instr		Line of code.
// @return			Number of microops.
inline int nom(const string &line, unordered_map<string,uint16_t> &d_lbl) {
	int c = 0;
    // Parse command
    string instr = line.substr(0, 3); // Instruction
    bool s = false; // 's' flag (used for ALU operations)
    cond cnd = AL; // Conditional
    if (line[3] == 's') {
        s = true;
        if (line[4] != ' ') cnd = condition(line.substr(4, 6));
    } else {
        if (line[3] != ' ') cnd = condition(line.substr(3, 5));
    }
    // Parse arguments
    vector<string> tmp = split(line, ':'), args;
    if (tmp.size() > 1) {
        tmp[1] = trim(tmp[1]);
        args = split(tmp[1], ',');
        for (string &arg : args) arg = trim(arg);
    }
	// Complex commands
	if (instr.compare("put") == 0) {
		uint16_t addr = d_lbl[args[1].substr(1)], a1 = addr >> 12, a2 = (addr >> 6) & 0x3f, a3 = addr & 0x3f;
		if (a1 != 0x0) c += 3;
		if (a2 != 0x0) c += 3;
		if (a1 != 0x0 || a2 != 0x0) c += 3;
		if (a3 != 0x0) c += 2;
		c += 2;
	// Four/seven-microop commands
	} else if (instr.compare("add") == 0 || instr.compare("sub") == 0 || instr.compare("and") == 0 || instr.compare("orr") == 0 || instr.compare("eor") == 0 || instr.compare("lsl") == 0 || instr.compare("lsr") == 0 || instr.compare("asr") == 0) {
		if (s && cnd != AL) c += 3;
		c += 4;
    // Three-microop commands
	} else if (instr.compare("cmp") == 0) c += 3;
	// Two-microop commands
	else if (instr.compare("ldr") == 0 || instr.compare("str") == 0) c += 2;
	// One-microop commands
	else if (instr.compare("set") == 0 || instr.compare("mov") == 0 || instr.compare("prt") == 0 || instr.compare("jmp") == 0 || instr.compare("nop") == 0 || instr.compare("hlt") == 0) ++c;
	// Unknown command
	else throw invalid_argument("Unknown instruction.");
	return c;
}

// Parse a single line of code (a single command)
// @param line      Line of code.
// @return          Corresponding command.
inline string parseLine(const string &line, unordered_map<string,uint16_t> &d_lbl, unordered_map<string,uint16_t> &p_lbl) {
    oss os;
    string instr = line.substr(0, 3); // Instruction
    bool s = false; // 's' flag (used for ALU operations)
    cond c = AL; // Conditional
    if (line[3] == 's') {
        s = true;
        if (line[4] != ' ') c = condition(line.substr(4, 2));
    } else {
        if (line[3] != ' ') c = condition(line.substr(3, 2));
    }
    // Parse arguments
    vector<string> tmp = split(line, ':'), args;
    if (tmp.size() > 1) {
        tmp[1] = trim(tmp[1]);
        args = split(tmp[1], ',');
        for (string &arg : args) arg = trim(arg);
    }
    // Decode instruction
    if (instr.compare("put") == 0) { // PUT instruction
        if (args.size() < 2) throw invalid_argument("Too few arguments.");
        uint16_t imm_val;
        if (args[1][0] == '$') {
            if (d_lbl.find(args[1].substr(1)) == d_lbl.end()) throw invalid_argument("Invalid label.");
            imm_val = d_lbl[args[1].substr(1)];
        } else imm_val = (uint16_t)atoi(args[1].c_str());
        os << put(regst(args[0]), imm_val, c);
    } else if (instr.compare("set") == 0) { // SET instruction
        if (args.size() < 2) throw invalid_argument("Too few arguments.");
        int imm_val;
        if (args[1][0] == '$') {
            if (d_lbl.find(args[1].substr(1)) == d_lbl.end()) throw invalid_argument("Invalid label.");
            imm_val = d_lbl[args[1].substr(1)];
        } else imm_val = (uint16_t)atoi(args[1].c_str());
        try {
            os << set(regst(args[0]), imm_val, c);
        } catch (invalid_argument &e) {
            throw e;
        } catch (out_of_range &e) {
            throw e;
        }
    } else if (instr.compare("mov") == 0) { // MOV instruction
        if (args.size() < 2) throw invalid_argument("Too few arguments.");
        os << mov(regst(args[1]), regst(args[0]), c);
    } else if (instr.compare("ldr") == 0) { // LDR instruction
        if (args.size() < 2) throw invalid_argument("Too few arguments.");
        os << ldr(regst(args[0]), regst(args[1]), c);
    } else if (instr.compare("str") == 0) { // STR instruction
        if (args.size() < 2) throw invalid_argument("Too few arguments.");
        os << str(regst(args[1]), regst(args[0]), c);
    } else if (instr.compare("add") == 0) { // ADD instruction
        if (args.size() < 3) throw invalid_argument("Too few arguments.");
        os << add(regst(args[1]), regst(args[2]), regst(args[0]), s, c);
    } else if (instr.compare("sub") == 0) { // SUB instruction
        if (args.size() < 3) throw invalid_argument("Too few arguments.");
        os << sub(regst(args[1]), regst(args[2]), regst(args[0]), s, c);
    } else if (instr.compare("and") == 0) { // AND instruction
        if (args.size() < 3) throw invalid_argument("Too few arguments.");
        os << _and(regst(args[1]), regst(args[2]), regst(args[0]), s, c);
    } else if (instr.compare("orr") == 0) { // ORR instruction
        if (args.size() < 3) throw invalid_argument("Too few arguments.");
        os << orr(regst(args[1]), regst(args[2]), regst(args[0]), s, c);
    } else if (instr.compare("eor") == 0) { // EOR instruction
        if (args.size() < 3) throw invalid_argument("Too few arguments.");
        os << eor(regst(args[1]), regst(args[2]), regst(args[0]), s, c);
    } else if (instr.compare("lsl") == 0) { // LSL instruction
        if (args.size() < 3) throw invalid_argument("Too few arguments.");
        os << lsl(regst(args[1]), regst(args[2]), regst(args[0]), s, c);
    } else if (instr.compare("lsr") == 0) { // LSR instruction
        if (args.size() < 3) throw invalid_argument("Too few arguments.");
        os << lsr(regst(args[1]), regst(args[2]), regst(args[0]), s, c);
    } else if (instr.compare("asr") == 0) { // ASR instruction
        if (args.size() < 3) throw invalid_argument("Too few arguments.");
        os << asr(regst(args[1]), regst(args[2]), regst(args[0]), s, c);
    } else if (instr.compare("prt") == 0) { // PRT instruction
        if (args.size() < 1) throw invalid_argument("Too few arguments.");
        os << prt(regst(args[0]), c);
    } else if (instr.compare("cmp") == 0) { // CMP instruction
        if (args.size() < 2) throw invalid_argument("Too few arguments.");
        os << cmp(regst(args[0]), regst(args[1]), c);
    } else if (instr.compare("jmp") == 0) { // JMP instruction
        if (args.size() < 1) invalid_argument("Too few arguments.");
        uint16_t addr;
        if (args[0][0] == '$') { // Label
            if (p_lbl.find(args[0].substr(1)) == p_lbl.end()) throw invalid_argument("Invalid label.");
            addr = p_lbl[args[0].substr(1)];
        } else if (args[0][1] == 'x') addr = (uint16_t)stoi(args[0], 0, 16); // Hexadecimal address
        else addr = (uint16_t)atoi(args[0].c_str()); // Decimal address
        addr -= mem_iprg; // 'addr' is an offset inside code segment
        try {
            os << jmp(addr);
        } catch (out_of_range &e) {
            throw e;
        }
    } else if (instr.compare("nop") == 0) { // NOP instruction
        os << nop(c);
    } else if (instr.compare("hlt") == 0) { // HLT instruction
        os << hlt(c);
    } else {
        throw invalid_argument("Unknown instruction.");
    }
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
        if (line.compare(".data") == 0) { sec = 1; ptr = mem_idat; continue; } // Start of data section
        else if (line.compare(".prgm") == 0) { sec = 2; eod = ptr; ptr = mem_iprg; continue; } // Start of prgm section
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
                    cerr << "Error on line " << c << ": " << e.what() << nl;
                }
            }
        }
    }
    if (mem_iprg < eod) cerr << "Error: data section exceeds ~32KB and therefore program cannot be compiled." << nl;
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
            if (line[0] == '&') continue; // Skip labels, they've already been computed
            else {
                try {
                    bin << parseLine(line, d_lbl, p_lbl); // Parse single line
                } catch (invalid_argument &e) {
                    cerr << "Error on line " << c << ": " << e.what() << nl;
                } catch (out_of_range &e) {
                    cerr << "Error on line " << c << ": " << e.what() << nl;
                }
            }
        }
    }
    prg.close();
    return bin.str();
}

#endif