#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Commands, ..
#define COMPILER         "g++"
#define FLAGS            "-Wall -std=c++2b -fmodules-ts -O3"
#define CPP_HEADER_FLAGS "-x c++-system-header"
#define TO_BIN           "-o"
#define TO_OBJ           "-c"
#define RENAME           "-o"

#define BUILD     "_"
#define OBJS      "O"
#define SRC       "source"
#define MAIN      "main.c++"
#define CINFINITY "Ci"

#if defined(_WIN32) || defined(_WIN64)
    #define WIN
    #define TEST_SCRIPT ".\\std\\main.ci"
    #define PATH_SEP "\\"
#else
    #define TEST_SCRIPT "./std/main.ci"
    #define PATH_SEP "/"
#endif

// Object files
#define LIB      "lib"
#define ANALYZER "analyzer"

vector<string> path_files
 =
{
    // 'lib' (folder)
    LIB PATH_SEP "file",

    // 'analyzer' (folder)
    ANALYZER PATH_SEP "scanner",
    ANALYZER PATH_SEP "parser",
    ANALYZER PATH_SEP "error",
};

// Standard headers (library)
vector<string> path_stdhs
 =
{
    //"iostream",
};

// --- //

// Command ('run'): compile + run | run-only
#define RUN__COMPILE   true
#define RUN__UNCOMPILE false

// Prototypes
void   error     (string);
void   cmd_new   (void);
void   cmd_run   (bool);
void   cmd_void  (void);
void   cmd_header(string);
string cmd_objs  (string, uint_fast64_t&);

// Compilation: 'g++ -o new newfile.c++'
int main(int argc, char* argv[])
{
    // Command: 'new' (compile-only)
    if (argc <= 1 || string(argv[1]) == "")
    {
        // Error: unexpected command (only 1-argument)
        if (argc > 2) { error(string("unexpected command { ") + argv[2] + " }"); }

        cmd_new();
        cout << "new { " CINFINITY " }\n";
    }
    // Command: 'new run' (run-only)
    else if (string(argv[1]) == "run")
    {
        // Error: unexpected command (only 1-argument)
        if (argc > 2) { error(string("unexpected command { ") + argv[2] + " }"); }

        cmd_run(RUN__UNCOMPILE);
    }
    // Command: 'new prerun' (compile + run)
    else if (string(argv[1]) == "prerun")
    {
        // Error: unexpected command (only 1-argument)
        if (argc > 2) { error(string("unexpected command { ") + argv[2] + " }"); }

        cmd_run(RUN__COMPILE);
    }
    // Command: 'new void' (clean directory)
    else if (string(argv[1]) == "void")
    {
        cmd_void();
    }
    else { error(string("illegal command { ") + argv[1] + " }"); } // Command: illegal
}

// Show error (diagnosis) and stop all
void error(string msg = "") { cerr << "error: " << msg << "\n"; exit(EXIT_FAILURE); }

// Compile + run | run-only (C∞)
void cmd_run(bool compile = RUN__COMPILE)
{
    // Compile: C∞
    if (compile == RUN__COMPILE) { cmd_new(); }

    // Run: C∞

    int err = 0; string run_cinfinity = "." PATH_SEP BUILD PATH_SEP CINFINITY " \"" TEST_SCRIPT "\"";
    cout << "new { " CINFINITY " } + run\n--\n";
    err = system(run_cinfinity.c_str());

    // Error when running
    if (err != 0) { cerr << "--\n"; error(string("program execution failure { ") + run_cinfinity + " }"); }
}

// Compile-only (C∞)
void cmd_new(void)
{
    int err = 0;

    // Create folder: build '_'

    #if defined(WIN)
        string create_build_folder = "IF not exist ." PATH_SEP BUILD " ( mkdir ." PATH_SEP BUILD " )";
        err = system(create_build_folder.c_str());

        // Error creating folder: build '_'
        if (err != 0) { error("the folder could not be created { ." PATH_SEP BUILD " }"); }
    #endif

    // Compile: C∞

    // Compile only: headers
    for (string path : path_stdhs) { cmd_header(path); }

    // Compile only: objects
    string objs = ""; uint_fast64_t objs_i = 0;
    for (string path : path_files) { objs += " ." PATH_SEP BUILD PATH_SEP + cmd_objs(path, objs_i); }

    // Compile all (main + objects)

    string cmd = COMPILER " " FLAGS " " TO_BIN " ." PATH_SEP BUILD PATH_SEP CINFINITY " ." PATH_SEP SRC PATH_SEP MAIN + objs;
    err = system(cmd.c_str());

    // Error compiling (binary)
    if (err != 0) { error(string("the binary could not be compiled { ") + cmd + " }"); }
}

// Compile 1-file: '.c++', to object: '.o'
string cmd_objs(string path, uint_fast64_t &objs_i)
{
    string cpp   = path + ".c++";
    string cpp_o = to_string(objs_i++) + ".o";
    int err = 0;

    // Compile: object '.o'

    string cmd_obj = string(COMPILER " " FLAGS " " TO_OBJ " ." PATH_SEP SRC PATH_SEP) + cpp
        + " " RENAME " ." PATH_SEP BUILD PATH_SEP + cpp_o;
    err = system(cmd_obj.c_str());

    // Error compiling (object)
    if (err != 0) { error(string("object could not be compiled { ") + cmd_obj + " }"); }

    return cpp_o;
}

// Clean directory
void cmd_void(void)
{
    int err = 0;

    // Remove folder: build '_'

    #if defined(WIN)
        string remove_build_folder = "IF exist ." PATH_SEP BUILD " ( rmdir /s /q ." PATH_SEP BUILD " )";
        err = system(remove_build_folder.c_str());

        // Error removing folder: '_'
        if (err != 0) { error("the folder could not be removed { ." PATH_SEP BUILD " }"); }
        else { cout << "new void { " CINFINITY ":" BUILD " }\n"; }
    #endif

    // Remove folder: 'gcm.cache'

    #define GCM_CACHE "gcm.cache"

    #if defined(WIN)
        string remove_gcm_cache_folder = "IF exist ." PATH_SEP GCM_CACHE " ( rmdir /s /q ." PATH_SEP GCM_CACHE " )";
        err = system(remove_gcm_cache_folder.c_str());

        // Error removing folder: 'gcm.cache'
        if (err != 0) { error("the folder could not be removed { ." PATH_SEP GCM_CACHE " }"); }
        else { cout << "new void { " CINFINITY ":" GCM_CACHE " }\n"; }
    #endif
}

// Compile 1-header 'import <header>' (standard library)
void cmd_header(string path_str = "")
{
    int err = 0;

    // Compile only: headers
    string cmd_header = COMPILER " " FLAGS " " TO_OBJ " " CPP_HEADER_FLAGS " " + path_str;
    err = system(cmd_header.c_str());

    // Error compiling (headers)
    if (err != 0) { error(string("header could not be compiled { ") + cmd_header + " }"); }
}
