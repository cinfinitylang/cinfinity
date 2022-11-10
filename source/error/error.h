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

#define PREFIX__ERROR   "error"
#define PREFIX__WARNING "warning"

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
        COORD cursor_position;
        std::uint_fast64_t prefix_size      = 0,
                           indent           = 0, i = 0,
                           line_number_size = std::to_string(self.line_number).size()+1; // +1 = space ' ',
                                                                                         // at the beginning:
                                                                                         // error  | path/file
                                                                                         // ' '100 | error line
                                                                                         //          ↑ 1: error message

        // Line 1: 'error | path/file' //

        // Is: error
        if (type_problem == PROBLEM__WARNING)
        {
            #if defined(OS_WIN)
                SetConsoleTextAttribute(win_console, ERROR__COLOR__WARNING);
            #endif
            std::cout << PREFIX__WARNING;
            prefix_size = std::string(PREFIX__WARNING).size(); // Size: "warning"
        }
        // Is: warning
        else
        {
            #if defined(OS_WIN)
                SetConsoleTextAttribute(win_console, ERROR__COLOR__ERROR);
            #endif
            std::cout << PREFIX__ERROR;
            prefix_size = std::string(PREFIX__ERROR).size(); // Size: "error"
        }

        // Size: line number '100000' (6), is greather that 'error' (5)
        if (prefix_size > line_number_size)
        {
            indent = prefix_size - line_number_size;
        }
        // Size: line number '100' (3), is less that 'error' (5)
        else if (prefix_size < line_number_size)
        {
            indent = line_number_size - prefix_size;
            for (i = 0; i < indent; ++i) { std::cout << " "; }
        }

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD_HIGH);
        #endif
        std::cout << " | ";

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD);
        #endif
        std::cout << self.path << "\n ";

        // Line 2: ' 10 | error line' //

        // Size: line number '100000' (6), is greather that 'error' (5)
        if (prefix_size > line_number_size)
        {
            indent = prefix_size - line_number_size;
            for (i = 0; i < indent; ++i) { std::cout << " "; }
        }
        // Size: line number '100' (3), is less that 'error' (5)
        else if (prefix_size < line_number_size)
        {
            indent = line_number_size - prefix_size;
        }

        // Is: warning
        if (type_problem == PROBLEM__WARNING)
        {
            #if defined(OS_WIN)
                SetConsoleTextAttribute(win_console, ERROR__COLOR__WARNING);
            #endif
        }
        // Is: error
        else
        {
            #if defined(OS_WIN)
                SetConsoleTextAttribute(win_console, ERROR__COLOR__ERROR);
            #endif
        }
        std::cout << self.line_number;

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD_HIGH);
        #endif
        std::cout << " | ";

        // Print error line
        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD);
        #endif
        scanner_t error_scanner; error_scanner.open(self.path, std::fstream::in);
        bool      skip_first_whitespaces = true;
        while (error_scanner.scan())
        {
            // Other line
            if (self.line_number != error_scanner.token.line_number) { continue; }

            // Is: error line //

            // Skip first whitespaces
            if ((error_scanner.token.id == TABLE__SPACE && skip_first_whitespaces) ||
                 error_scanner.token.id == TABLE__AUTO_SEMICOLON) { continue; }

            // Is: error token //
            if (self.char_number == error_scanner.token.char_number)
            {
                // Is: warning
                if (type_problem == PROBLEM__WARNING)
                {
                    #if defined(OS_WIN)
                        SetConsoleTextAttribute(win_console, ERROR__COLOR__WARNING);
                    #endif
                }
                // Is: error
                else
                {
                    #if defined(OS_WIN)
                        SetConsoleTextAttribute(win_console, ERROR__COLOR__ERROR);
                    #endif
                }

                // Get console-position of error token
                #if defined(OS_WIN)
                    CONSOLE_SCREEN_BUFFER_INFO console_info;
                    GetConsoleScreenBufferInfo(win_console, &console_info);
                    cursor_position = console_info.dwCursorPosition;
                    cursor_position.Y++;
                #endif

                std::cout << error_scanner.token.value;
                #if defined(OS_WIN)
                    SetConsoleTextAttribute(win_console, ERROR__COLOR__STD);
                #endif
            }
            // Other tokens in error line
            else
            {
                std::cout << error_scanner.token.value;
            }

            if (skip_first_whitespaces) { skip_first_whitespaces = false; }
        }
        std::cout << "\n";

        // Line 3: ' ..↑ 1: error message' //

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD_HIGH);
            SetConsoleCursorPosition(win_console, cursor_position);
        #endif
        std::cout << "^";

        // Reset to standard color (console)
        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD);
        #endif
    }
};
