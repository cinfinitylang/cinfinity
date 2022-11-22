module;

#include <iostream>

#include "./scanner.h"
#include "./table.h"
#include "./error.h" // Self

export module error;

#if defined(OS_WIN)
    #define ERROR__COLOR__STD      7
    #define ERROR__COLOR__STD_HIGH 15
    #define ERROR__COLOR__EXPECT   10
    #define ERROR__COLOR__ERROR    12
    #define ERROR__COLOR__WARNING  14
#endif

// Type of problem (error | warning)
#define PROBLEM__ERROR   true
#define PROBLEM__WARNING false

#define PREFIX__ERROR   "error"
#define PREFIX__WARNING "warning"

using namespace std;
using uint8 = uint_fast8_t;
using uint  = uint_fast64_t;
using fs    = fstream;

namespace cinfinity
{
    // Show error (diagnosis/message) and stop all
    void error::err(string message = "") { _problem(PROBLEM__ERROR, message); exit(EXIT_FAILURE); }

    // Show warning (diagnosis/message) and continue
    void error::warn(string message = "") { _problem(PROBLEM__WARNING, message); }

    // Show problem (error | warning) - (diagnosis/message with format)
    void error::_problem(bool problem_type = PROBLEM__ERROR, string message = "")
    {
        // Unexist: next-token
        if (next_token_exist == NEXT_TOKEN__UNEXIST) { token = token_helper; }

        #if defined(OS_WIN)
            CONSOLE_SCREEN_BUFFER_INFO console_info;
            COORD                      cursor_pos__token = {X: 0, Y: 0},
                                       cursor_pos__pleca = {X: 0, Y: 0};
        #endif

        // Format: line-number with thousands-separator ('100000000' → '100 000 000')
        string thousands_sep__linenum = _thousands_sep(to_string(token.linenum));

        uint prefix_size  = 0,
             indent       = 0, i = 0,
             linenum_size = thousands_sep__linenum.size()+1; // '+1' = 1-space: ' ', at the beginning:
                                                              // error  | path/file
                                                              // ' '100 | error line
                                                              //        | ↑ 1 | error message

        // LINE: 1 ('error | path/file')

        // Is: error
        if (problem_type == PROBLEM__WARNING)
        {
            _error_color(PREFIX__WARNING, ERROR__COLOR__WARNING);
            prefix_size = string(PREFIX__WARNING).size(); // Size: "warning"
        }
        // Is: warning
        else
        {
            _error_color(PREFIX__ERROR, ERROR__COLOR__ERROR);
            prefix_size = string(PREFIX__ERROR).size(); // Size: "error"
        }

        // Format: (get pairing-number)
        // --
        //  error''| path/file
        //   100' '| error line
        //         | ↑ 1 | error message
        // --
        // 'error'(5)   > '100'(3)    - (this is an example)
        if (prefix_size > linenum_size)
        {
            indent = prefix_size - linenum_size;
        }
        //      'error'(5)   < '10 000'(6) - (this is an example)
        else if (prefix_size < linenum_size)
        {
            indent = linenum_size - prefix_size;
            for (i = 0; i < indent; ++i) { cerr << " "; }
        }

        _error_color(" | ",        ERROR__COLOR__STD_HIGH);
        _error_color(path + "\n ", ERROR__COLOR__STD);

        // LINE: 2 (' 10 | error line')

        // Format: (get pairing-number)
        // --
        //  error'   '| path/file
        //   10 000' '| error line
        //            | ↑ 1 | error message
        // --
        // 'error'(5)   > '100'(3)    - (this is an example)
        if (prefix_size > linenum_size)
        {
            indent = prefix_size - linenum_size;
            for (i = 0; i < indent; ++i) { cerr << " "; }
        }
        //      'error'(5)   < '10 000'(6) - (this is an example)
        else if (prefix_size < linenum_size)
        {
            indent = linenum_size - prefix_size;
        }

        if   (problem_type == PROBLEM__WARNING) { _error_color(thousands_sep__linenum, ERROR__COLOR__WARNING); } // Is: warning
        else                                    { _error_color(thousands_sep__linenum, ERROR__COLOR__ERROR);   } // Is: error

        #if defined(OS_WIN)

            // Get console-position of error token
            GetConsoleScreenBufferInfo(win_console, &console_info);
            cursor_pos__pleca = console_info.dwCursorPosition;
            cursor_pos__pleca.X++;
        #endif
        _error_color(" | ", ERROR__COLOR__STD_HIGH);

        // Print: error-line
        _error_color("", ERROR__COLOR__STD);
        cinfinity::file    error__file(path, fs::in);
         error__file.linenum = 1;
         error__file.charnum = 0;
        cinfinity::scanner error__scanner;
        cinfinity::token   error__token;
         INIT_TOKEN(error__token);
        bool               skip_first_whitespaces = true;
        while (error__scanner.scan(error__file, error__token))
        {
            //  OTHER-LINE                            ||   Skip 1°-whitespace(s) - (in ERROR-LINE)
            if (token.linenum != error__token.linenum || ((error__token.id == table::SPACE && skip_first_whitespaces) ||
                error__token.id == table::AUTO_SEMICOLON)) { continue; }
            
            // ERROR-LINE //

            // ERROR-TOKEN
            if (token.charnum == error__token.charnum)
            {
                // Get: console-position of error-token
                #if defined(OS_WIN)
                    GetConsoleScreenBufferInfo(win_console, &console_info);
                    cursor_pos__token = console_info.dwCursorPosition;
                    cursor_pos__token.Y++;
                #endif

                if   (problem_type == PROBLEM__WARNING) { _error_color(error__token.val, ERROR__COLOR__WARNING); } // Is: warning
                else                                    { _error_color(error__token.val, ERROR__COLOR__ERROR);   } // Is: error

                // Reset color: to standard
                _error_color("", ERROR__COLOR__STD);
            }
            // Other-tokens - (in error-line)
            else { cerr << error__token.val; }

            // Unexist: next-token
            if (next_token_exist == NEXT_TOKEN__UNEXIST && error__token.charnum+1 == token.charnum)
            {
                _error_color(token.val, ERROR__COLOR__EXPECT);
                _error_color("",        ERROR__COLOR__STD); // Reset color: to standard
            }

            if (skip_first_whitespaces) { skip_first_whitespaces = false; }
        }
        cerr << "\n";

        // LINE: 3 (' ..| ..↑ 1 | error message')

        for (i = 0; i < (uint)cursor_pos__pleca.X; ++i) { cerr << ' '; }
        _error_color("| ", ERROR__COLOR__STD_HIGH);
        //           ↑--↑ →---------------------→ ↓ this: '+2' = +size("| ") - (previous)
        for (i = (((uint)cursor_pos__token.X >= (i+2))? (uint)cursor_pos__token.X - (i+2): token.charnum-1); i > 0 ; --i)
         { cerr << ' '; } cerr << "^ ";

        // Is: warning
        if (problem_type == PROBLEM__WARNING)
        {
            _error_color(_thousands_sep(to_string(token.charnum)), ERROR__COLOR__WARNING);
        }
        // Is: error
        else
        {
            _error_color(_thousands_sep(to_string(token.charnum)), ERROR__COLOR__ERROR);
        }

        _error_color(" | ",          ERROR__COLOR__STD_HIGH);
        _error_color(message + '\n', ERROR__COLOR__STD); // Reset color: to standard (console)
    }

    // Print (stderr) with-color
    void error::_error_color(string message = "", uint8 color_code = ERROR__COLOR__STD)
    {
        // Windows (32/64 bits)
        #if defined(OS_WIN)
            SetConsoleTextAttribute(win_console, color_code);
        #endif

        cerr << message;
    }

    // Thousands separator: '100 000'
    //                          ↑ space: ' '
    string error::_thousands_sep(string number_str = "")
    {
        string number__thousands_sep = "";
        uint8  thousands_sep_i       = 0;

        // Thousands-separator for numbers: '100000000' → '100 000 000'
        for (uint i = number_str.size(); i > 0; --i)
        {
            ++thousands_sep_i;

            // Add thousands-separator (' ')
            if (thousands_sep_i > 3) {
                number__thousands_sep.insert(0, 1, ' '); thousands_sep_i = 0;
                //                          ↗   ↑    ↖
                //                        ↙     ↓      ↘
                //                      Begin | Size  | Character (to append)
            }

            // Digit without-changes
            number__thousands_sep.insert(0, 1, number_str[i-1]);
        }

        return number__thousands_sep;
    }
}
