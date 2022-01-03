#include "Code.hpp"
#include <string>
#include <iostream>
using namespace std;

string Code::dest(string destString) {
    if (destString.compare("M") == 0) return "001";
    else if (destString.compare("D") == 0) return "010";
    else if (destString.compare("MD") == 0) return "011";
    else if (destString.compare("A") == 0) return "100";
    else if (destString.compare("AM") == 0) return "101";
    else if (destString.compare("AD") == 0) return "110";
    else if (destString.compare("AMD") == 0) return "111";
    else return "000";
}

string Code::comp(string compString) {
    string res;
    int i = compString.find("M");
    if (i >= 0) {
        compString.replace(i, 1, "A");
        res = "1";
    }
    else res = "0";

    if (compString.compare("0") == 0) res += "101010";
    else if (compString.compare("1") == 0) res += "111111";
    else if (compString.compare("-1") == 0) res += "111010";
    else if (compString.compare("D") == 0) res += "001100";
    else if (compString.compare("A") == 0) res += "110000";
    else if (compString.compare("!D") == 0) res += "001101";
    else if (compString.compare("!A") == 0) res += "110001";
    else if (compString.compare("-D") == 0) res += "001111";
    else if (compString.compare("-A") == 0) res += "110011";
    else if (compString.compare("D+1") == 0) res += "011111";
    else if (compString.compare("A+1") == 0) res += "110111";
    else if (compString.compare("D-1") == 0) res += "001110";
    else if (compString.compare("A-1") == 0) res += "110010";
    else if (compString.compare("D+A") == 0) res += "000010";
    else if (compString.compare("D-A") == 0) res += "010011";
    else if (compString.compare("A-D") == 0) res += "000111";
    else if (compString.compare("D&A") == 0) res += "000000";
    else if (compString.compare("D|A") == 0) res += "010101";
    else return "code comp error";
    return res;
}

string Code::jump(string jumpString) {
    if (jumpString.compare("JGT") == 0)   return "001";
    else if (jumpString.compare("JEQ") == 0)   return "010";
    else if (jumpString.compare("JGE") == 0)   return "011";
    else if (jumpString.compare("JLT") == 0)   return "100";
    else if (jumpString.compare("JNE") == 0)   return "101";
    else if (jumpString.compare("JLE") == 0)   return "110";
    else if (jumpString.compare("JMP") == 0)   return "111";
    else return "000";
}
