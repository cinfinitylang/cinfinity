#include <iostream>

#include "./scanner/scanner.h"

// Compilation: g++ -o Cinfinity ./source/main.c++ -Wall -std=c++2b
int main(int argc, char* argv[])
{
    scanner_t scanner;
    scanner.openfile(argv[1], std::fstream::in);
    scanner.mode = scanner.mode_getall;

    while (scanner.scan()) {
        std::cout << "[" << scanner.token.val << "] ";
    }

    scanner.closefile();
}
