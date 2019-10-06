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

string parsePrg(const string &src) {
    oss bin;
    ifs prg(src);
    // TODO
    prg.close();
    return bin.str();
}

#endif