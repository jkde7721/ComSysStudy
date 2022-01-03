#include "Parser.hpp"
#include "Code.hpp"
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <bitset>
using namespace std;

Parser::Parser(string fileName) {
    try {
        inputStream.open(fileName);
        outputStream.open("Add.hack");
    }
    catch (int error) {
        return;
    }
    
    Code code;
    string c;

    while (hasMoreCommands()) {
        this->type = advance();
        if (type.compare("A_COMMAND") == 0) {
            symbolString = symbol();
            int num = stoi(symbolString);
            c = bitset<16>(num).to_string();
            outputStream.write(c.c_str(),c.length());
            outputStream.write("\n", 1);
            cout << c << "\n";
        }
        else if (type.compare("C_COMMAND") == 0) {
            c = "111";
            destString = dest();
            compString = comp();
            jumpString = jump();
            c += code.comp(compString);
            c += code.dest(destString);
            c += code.jump(jumpString);
            outputStream.write(c.c_str(), c.length());
            outputStream.write("\n",1);
            cout << c << "\n";
        }
        else if (type.compare("L_COMMAND") == 0) {
            symbolString = symbol();
        }
        else if (type.compare("SKIP")==0){
        }
        else return;
    }

    inputStream.close();
    outputStream.close();
}

bool Parser::hasMoreCommands() {
    if (inputStream.eof()) return false;
    else return true;
}

string Parser::advance() {
    getline(inputStream, command);
    int i;
    if(command[0]=='\r') return "SKIP";
    for (i = 0; i<command.length(); i++)
        if (command[i]!=' ') break;
    if (i>=command.length())
        return "SKIP";
    else if (command[i] == '/') {
        if (i<command.length()-1 && command[i + 1] == '/')
            return "SKIP";
        else
            return "ERROR";
    }
    else if (command[i] == '(')
        return "L_COMMAND";
    else if (command[i] == '@')
        return "A_COMMAND";
    else
        return "C_COMMAND";
}

string Parser::symbol() {
    switch (command[0]) {
    case '@':
        return command.substr(1);
    case '(':
        return command.substr(1, command.length() - 2);
    default:
            return "";
    }
}

string Parser::dest() {
    int i = command.find("=");
    string res;
    if (i >= 0) {
        for (int idx = 0; idx<i; idx++)
            if (command[idx] != ' ')   res += command[idx];
    }
    return res;
}

string Parser::comp() {
    int i = command.find("=");
    int j = command.find(";");
    string res;

    if (j >= 0) {
        for (int idx = i + 1; idx<j; idx++)
            if (command[idx] != ' ') res += command[idx];
    }
    else {
        for (int idx = i + 1; idx < command.length()-1; idx++)//mac에서 \r 붙음
            if (command[idx] != ' ') res += command[idx];
    }
    return res;
}

string Parser::jump() {
    int j = command.find(";");
    string res;

    if (j >= 0) {
        for (int idx = j + 1; idx<command.length()-1; idx++)//mac에서 \r 붙음
            if (command[idx] != ' ') res += command[idx];
    }
    return res;
}
