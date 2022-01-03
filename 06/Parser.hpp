#ifndef Parser_hpp
#define Parser_hpp

#include <string>
#include <fstream>
using namespace std;

class Parser {
private:
    ifstream inputStream;
    ofstream outputStream;
    string type;
    string command;
    string symbolString;
    string destString;
    string compString;
    string jumpString;

public:
    Parser(string fileName);
    bool hasMoreCommands();
    string advance();
    string commandType();
    string symbol();
    string dest();
    string comp();
    string jump();
};

#endif /* Parser_hpp */
