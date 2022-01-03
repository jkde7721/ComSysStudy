#include "Parser.hpp"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    string fname = argv[1];
    Parser parser = Parser(fname);
}
