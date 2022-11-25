#include <iostream>

// Windows (32/64 bits)
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WIN
#endif

#include "./lib/file.h"
#include "./lib/token.h"
#include "./analyzer/scanner.h"
#include "./analyzer/parser.h"
#include "./analyzer/error.h"
#include "./analyzer/table.h"

// Compilation: 'new' | 'new run'
int main(int argc, char* argv[])
{
    std::string path_file = "./std/main.ci";
    std::string path_cgen = "./std/main.cgen";

    cinfinity::file file(path_file, std::fstream::in);
     file.linenum = 1;
     file.charnum = 0;
    std::ofstream cgen(path_cgen);

    cinfinity::scanner scanner;
    cinfinity::token   token;
     INIT_TOKEN(token);

    cinfinity::error error;
     INIT_ERROR(error);
     INIT_TOKEN(error.token);
     INIT_TOKEN(error.token_helper);
     INIT_TOKEN(error.expected_token);
     error.path = "./std/main.ci";
     #if defined(OS_WIN)
        error.win_console = CONSOLE;
     #endif
    
    cinfinity::parser parser(scanner, file, token, cgen, error);
    parser.path_cgen = path_cgen;

    // Analyze: C∞-code + get code
    parser.parse();
    
    cgen.close(); // Close: file '.cgen'
}
