#include <iostream>
#include <fstream>

#define self (*this)

#include "./scanner/scanner.h"

// Compilation: g++ -o C ./source/main.c++ -Wall -std=c++2b
int main(int argc, char* argv[])
{
    scanner_t scanner(std::string(argv[1]), std::fstream::in);

    while (scanner.scan())
    {
        std::cout << "[" << scanner.token.value << "] ";
    }
}
