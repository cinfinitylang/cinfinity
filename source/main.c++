#include <iostream>
#include <fstream>

#define self (*this)

// Is: windows (32/64 bits)
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WIN
#endif

#include "./parser/parser.h"

// Compilation: g++ -o C ./source/main.c++ -Wall -std=c++2b
int main(int argc, char* argv[])
{
    parser_t parser(std::string(argv[1]), std::fstream::in);
    parser.parse();
}
