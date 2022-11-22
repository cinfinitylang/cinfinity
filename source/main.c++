#include <fstream>

#define self (*this)

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
    std::ofstream cgen("./std/main.cgen");
    cinfinity::file file("./std/main.ci", std::fstream::in);
     file.linenum = 1;
     file.charnum = 0;

    cinfinity::token  token;
     INIT_TOKEN(token);
    cinfinity::scanner scanner;
    cinfinity::parser  parser;

    cinfinity::error error;
     INIT_ERROR(error);
     INIT_TOKEN(error.token);
     INIT_TOKEN(error.token_helper);
     error.path = "./std/main.ci";
     #if defined(OS_WIN)
        error.win_console = CONSOLE;
     #endif

    // Analyze: C∞-code + get code
    parser.parse(scanner, file, token, cgen, error);
    
    cgen.close(); // Close: file '.cgen'
}
