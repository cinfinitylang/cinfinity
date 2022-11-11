#pragma once

#include <iostream>
#include <filesystem>
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
    private: void problem(bool problem_type = PROBLEM__ERROR, std::string message = "")
    {
        CONSOLE_SCREEN_BUFFER_INFO console_info;
        COORD                      cursor_position_token, cursor_position_pleca;

        // Format: line number, with space ' ' as thousands separator - ('100 000 000')
        std::string fmt_line_number = self.thousands_separator(std::to_string(self.line_number));

        std::uint_fast64_t prefix_size      = 0,
                           indent           = 0, i = 0,
                           line_number_size = fmt_line_number.size()+1; // +1 = space ' ',
                                                                        // at the beginning:
                                                                        // error  | path/file
                                                                        // ' '100 | error line
                                                                        //          ↑ 1: error message

        // Line 1: 'error | path/file' //

        // Is: error
        if (problem_type == PROBLEM__WARNING)
        {
            #if defined(OS_WIN)
                SetConsoleTextAttribute(win_console, ERROR__COLOR__WARNING);
            #endif
            std::cerr << PREFIX__WARNING;
            prefix_size = std::string(PREFIX__WARNING).size(); // Size: "warning"
        }
        // Is: warning
        else
        {
            #if defined(OS_WIN)
                SetConsoleTextAttribute(win_console, ERROR__COLOR__ERROR);
            #endif
            std::cerr << PREFIX__ERROR;
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
            for (i = 0; i < indent; ++i) { std::cerr << " "; }
        }

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD_HIGH);
        #endif
        std::cerr << " | ";

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD);
        #endif
        std::cerr << self.path << "\n ";

        // Line 2: ' 10 | error line' //

        // Size: line number '100000' (6), is greather that 'error' (5)
        if (prefix_size > line_number_size)
        {
            indent = prefix_size - line_number_size;
            for (i = 0; i < indent; ++i) { std::cerr << " "; }
        }
        // Size: line number '100' (3), is less that 'error' (5)
        else if (prefix_size < line_number_size)
        {
            indent = line_number_size - prefix_size;
        }

        // Is: warning
        if (problem_type == PROBLEM__WARNING)
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
        std::cerr << fmt_line_number;

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD_HIGH);

            // Get console-position of error token
            GetConsoleScreenBufferInfo(win_console, &console_info);
            cursor_position_pleca = console_info.dwCursorPosition;
            cursor_position_pleca.X++; cursor_position_pleca.Y++;
        #endif
        std::cerr << " | ";

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
                if (problem_type == PROBLEM__WARNING)
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
                    GetConsoleScreenBufferInfo(win_console, &console_info);
                    cursor_position_token = console_info.dwCursorPosition;
                    cursor_position_token.Y++;
                #endif

                std::cerr << error_scanner.token.value;
                #if defined(OS_WIN)
                    SetConsoleTextAttribute(win_console, ERROR__COLOR__STD);
                #endif
            }
            // Other tokens in error line
            else
            {
                std::cerr << error_scanner.token.value;
            }

            if (skip_first_whitespaces) { skip_first_whitespaces = false; }
        }
        std::cerr << "\n";

        // Line 3: ' ..↑ 1: error message' //

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD_HIGH);
            SetConsoleCursorPosition(win_console, cursor_position_pleca);
        #endif
        std::cerr << "|";
        #if defined(OS_WIN)
            SetConsoleCursorPosition(win_console, cursor_position_token);
        #endif
        std::cerr << "^ ";

        // Is: warning
        if (problem_type == PROBLEM__WARNING)
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
        // Format: character number, with space ' ' as thousands separator - ('100 000 000')
        std::cerr << self.thousands_separator(std::to_string(self.char_number));

        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD_HIGH);
        #endif
        std::cerr << " | ";

        // Reset to standard color (console)
        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, ERROR__COLOR__STD);
        #endif
        std::cerr << message << "\n";
    }

    std::string thousands_separator(std::string source = "")
    {
        std::string       fmt_source                = "";
        std::uint_fast8_t thousands_separator_count = 0;

        // Format: line number, with space ' ' as thousands separator - ('100 000 000')
        for (std::uint_fast64_t i = source.size(); i > 0; --i)
        {
            ++thousands_separator_count;

            // Add thousands separator: ' ', to line number
            if (thousands_separator_count > 3) {
                fmt_source.insert(0, " "); thousands_separator_count = 0;
            }

            fmt_source.insert(0, 1, source[i-1]);
        }

        return fmt_source;
    }
};
