#include <iostream>
#include <cstdlib>

using namespace std;

#define COMPILER "g++"
#define FLAGS    "-Wall -std=c++2b"
#define TO_BIN   "-o"
#define TO_OBJ   "-c"
#define RENAME   "-o"

#define BUILD     "BUILD"
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

// Prototypes
void error(string);
void cmd_new(void);
void cmd_run(void);
void cmd_void(void);

// Compilation: g++ -o new newfile.c++
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
    // Command: 'new run' (compile + run)
    else if (string(argv[1]) == "run")
    {
        // Error: unexpected command (only 1-argument)
        if (argc > 2) { error(string("unexpected command { ") + argv[2] + " }"); }

        cmd_run();
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

// Compile + run (C∞)
void cmd_run(void)
{
    // Compile: C∞
    cmd_new();

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

    // Create folder: 'BUILD'

    #if defined(WIN)
        string create_build_folder = "IF not exist ." PATH_SEP BUILD " ( mkdir ." PATH_SEP BUILD " )";
        err = system(create_build_folder.c_str());

        // Error creating folder: 'BUILD'
        if (err != 0) { error("the folder could not be created { ." PATH_SEP BUILD " }"); }
    #endif

    // Compile: C∞

    string cmd = COMPILER " " FLAGS " " TO_BIN " ." PATH_SEP BUILD PATH_SEP CINFINITY " ." PATH_SEP SRC PATH_SEP MAIN;
    err = system(cmd.c_str());

    // Error compiling
    if (err != 0) { error(string("failed to compile { ") + cmd + " }"); }
}

// Clean directory
void cmd_void(void)
{
    int err = 0;

    // Remove folder: 'BUILD'

    #if defined(WIN)
        string remove_build_folder = "IF exist ." PATH_SEP BUILD " ( rmdir /s /q ." PATH_SEP BUILD " )";
        err = system(remove_build_folder.c_str());

        // Error removing folder: 'BUILD'
        if (err != 0) { error("the folder could not be removed { ." PATH_SEP BUILD " }"); }
        else { cout << "new void { " CINFINITY " }\n"; }
    #endif
}
