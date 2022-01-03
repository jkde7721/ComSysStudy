#include "Parser.hpp"
#include "Code.hpp"
#include "SymbolTable.hpp"
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>//atoi
#include <cctype>
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
    
    ROM = 0;
    RAM = 16;
    Code code;
    SymbolTable st;
    string c;
    
    //1
    while(hasMoreCommands()){
        type = advance();
        if (type.compare("L_COMMAND") == 0) {
            symbolString = symbol();
            st.addEntry(symbolString, ROM);
        }
        else if (type.compare("A_COMMAND") == 0 || type.compare("C_COMMAND") == 0) {
            ROM++;
        }
    }
    
    //2
    inputStream.clear();
    inputStream.seekg(0);
    while (hasMoreCommands()) {
        type = advance();
        if (type.compare("A_COMMAND") == 0) {
            symbolString = symbol();
            int num = atoi(symbolString.c_str());
            if(num==0 && symbolString.compare("0")!=0){//기호
                if(st.contains(symbolString)) num = st.GetAddress(symbolString);
                else{
                    st.addEntry(symbolString, RAM);
                    num = RAM;
                    RAM++;
                }
            }
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
        }
        else if (type.compare("SKIP")==0){
        }
        else cout << "ERROR\n";
    }

    inputStream.close();
    outputStream.close();
}

bool Parser::hasMoreCommands() {
    if (inputStream.eof()) return false;
    else return true;
}

string Parser::advance() {
    getline(inputStream, line);
    command="";
    for(int i=0;i<line.length();i++){
        if(!isspace(line[i])) command+=line[i];
    }
    if(command.length()==0) return "SKIP";
    else if(command.length()>=2 && command[0]=='/' && command[1]=='/') return "SKIP";
    else if(command[0]=='(') return "L_COMMAND";
    else if(command[0]=='@') return "A_COMMAND";
    else return "C_COMMAND";
}

string Parser::symbol() {
    int i = command.find("@");
    int j = command.find("(");

    if(i>=0){
        return command.substr(i+1, command.length());
    }
    else
        return command.substr(j+1, command.length()-2);
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
        int k = command.find("/");
        if(k>i) res=command.substr(i+1,k-2);
        else res=command.substr(i+1);
    }
    return res;
}

string Parser::jump() {
    int j = command.find(";");
    string res;

    if (j >= 0) {
        int k = command.find("/");
        if(k>j) res=command.substr(j+1,k-2);
        else res=command.substr(j+1);
    }
    return res;
}
