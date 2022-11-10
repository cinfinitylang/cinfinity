#pragma once

#include <iostream>
#if defined(OS_WIN)
    #include <windows.h>

    #define ERROR__COLOR__STD      7
    #define ERROR__COLOR__STD_HIGH 15
    #define ERROR__COLOR__ERROR    12
    #define ERROR__COLOR__WARNING  14
#endif

#include "../scanner/scanner.h"
#include "../props/token_t.h"

// Type of problem (error | warning)
#define PROBLEM__ERROR   true
#define PROBLEM__WARNING false

struct error_t
{
    std::string path = "";
    token_t     token;

    #if defined(OS_WIN)
        HANDLE win_console = GetStdHandle(STD_ERROR_HANDLE);
    #endif

    std::uint_fast64_t line_number = 1;
    std::uint_fast64_t char_number = 0;

    // Throw error (message / diagnosis) + stop all
    void error(std::string message = "")
    {
        self.problem(PROBLEM__ERROR, message); std::exit(EXIT_FAILURE);
    }

    // Throw warning (message / diagnosis) + continue
    void warning(std::string message = "")
    {
        self.problem(PROBLEM__WARNING, message);
    }

    // Throw problem (error | warning) - (message/diagnosis with format)
    private: void problem(bool type_problem, std::string message = "")
    {
        // Is: error
        if (type_problem == PROBLEM__WARNING)
        {
            #if defined(OS_WIN)
                SetConsoleTextAttribute(win_console, ERROR__COLOR__WARNING);
            #endif
            std::cout << "warning: ";
        }
        // Is: warning
        else
        {
            #if defined(OS_WIN)
                SetConsoleTextAttribute(win_console, ERROR__COLOR__ERROR);
            #endif
            std::cout << "error: ";
        }

        // Reset to standard color (console)
        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD);
        #endif
    }
};
