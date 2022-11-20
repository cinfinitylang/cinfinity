#include <iostream>
#include <fstream>
#include <vector>

#define self (*this)

// Is: windows (32/64 bits)
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WIN
#endif

#include "./parser/parser.h"

// Compilation: 'g++ -o Ci ./source/main.c++ -Wall -std=c++2b'
int main(int argc, char* argv[])
{
    // Analyze: C∞-code and generate C++-code (write in files) //

    std::fstream file; file.open("./std/main.c++", std::ios::out);
    if (!file.is_open())
    {
        std::cerr << "error: file not opened" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    parser_t parser({std::string((char*)argv[1])});
    parser.parse(file);
    file.close();
}
