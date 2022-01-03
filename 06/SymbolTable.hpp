#ifndef SymbolTable_hpp
#define SymbolTable_hpp

#include <string>
#include <unordered_map>
using namespace std;

class SymbolTable{
private:
    unordered_map<string,int> table;
    string symbol;
    
public:
    SymbolTable();
    void addEntry(string symbol, int address);
    bool contains(string symbol);
    int GetAddress(string symbol);
};

#endif /* SymbolTable_hpp */
