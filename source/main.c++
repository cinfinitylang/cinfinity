#include <iostream>
#include <vector>

#define self (*this)

// Is: windows (32/64 bits)
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WIN
#endif

//#include "./parser/parser.h"
#include "./stdlib/file.h"

// Compilation: 'new' | 'new run'
int main(int argc, char* argv[])
{
    cinfinity::file fs("./std/main.ci", std::fstream::in);
    while (fs.contains())
    {
        std::cout << "[" << fs.get_unicode() << "] ";
    }

    return EXIT_SUCCESS;

    // Analyze: C∞-code and generate C++-code (write in files) //

    std::fstream file; file.open("./std/main.c++", std::ios::out);
    if (!file.is_open())
    {
        std::cerr << "error: file not opened" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    //parser_t parser({std::string((char*)argv[1])});
    //parser.parse(file);
    file.close();
}
