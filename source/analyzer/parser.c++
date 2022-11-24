module;

#include <iostream>

#include "../lib/file.h"
#include "../lib/token.h"
#include "./scanner.h"
#include "./table.h"
#include "./error.h"
#include "./parser.h" // Self

export module parser;

using namespace std;
using uint8  = uint_fast8_t;
using string = string;
using fs     = ofstream;

// Syntactic analyzer for 1-file
export namespace cinfinity
{
    // Analyze 1-file (all)
    void parser::parse(cinfinity::scanner &scanner, cinfinity::file &file, cinfinity::token &token, fs &cgen,
        cinfinity::error &error)
    {
        recursive_parse(scanner, file, token, cgen, error, scope::GLOBAL);
    }

    // Analyze next-sentence in file
    void parser::recursive_parse(cinfinity::scanner &scanner, cinfinity::file &file, cinfinity::token &token, fs &cgen,
        cinfinity::error &error, uint8 scope = scope::NESTED)
    {
        while (_scan_file(scanner, file, token, cgen, error, SCAN_CLEAN))
        {
            // Sentence: 'ft name(..): (..) { .. }'
            if (token.id == table::KEYWORD__FT)
            {
                // ERROR: illegal scope (for 'ft')
                if (scope != scope::GLOBAL) { _error("illegal scope for 'ft'", error, cgen); }

                cgen << "!ft:\n";
                error.token_helper = token; // Previous-token (helper for error-handler)

                // IS: name ('ft name(..): (..) { .. }')
                //               ↑--↑ name

                // ERROR: unexist
                if (!_scan_file(scanner, file, token, cgen, error, SCAN_CLEAN))
                {
                    error.expected_token     = error.token_helper;
                    error.expected_token.id  = table::NAME;
                    error.expected_token.val = "name";
                    _error_expected_token(error);

                    _error("expected name for 'ft'", error, cgen);
                }
                // ERROR: illegal
                if (token.id != table::NAME) { _error("illegal name for 'ft'", error, cgen); }
                error.token_helper = token; // Previous-token
                _token_write(cgen, token); // Token: name

                // IS: open-argument(s) ('ft name(..): (..) { .. }')
                //                               ↑ '('

                // ERROR: unexist
                if (!_scan_file(scanner, file, token, cgen, error, SCAN_CLEAN))
                {
                    error.expected_token.id  = table::OPEN_PAREN;
                    error.expected_token.val = '(';
                    _error_expected_token(error);

                    _error("expected open arguments for 'ft'", error, cgen);
                }
                // ERROR: illegal
                if (token.id != table::OPEN_PAREN) { _error("illegal open-argument(s) for 'ft'", error, cgen); }
                cgen << "!args:\n"; // Open arguments: ft name '('..) { .. }

                // IS: close-argument(s) ('ft name(..): (..) { .. }')
                //                                   ↑ ')'

                // ERROR: unexist
                if (!_scan_file(scanner, file, token, cgen, error, SCAN_CLEAN))
                {
                    error.expected_token.id  = table::CLOSE_PAREN;
                    error.expected_token.val = ')';
                    _error_expected_token(error);

                    _error("expected close arguments for 'ft'", error, cgen);
                }
                // ERROR: illegal
                if (token.id != table::CLOSE_PAREN && token.id != table::NAME)
                 { _error("illegal argument name or close arguments for 'ft'", error, cgen); }

                // Arguments
                if (token.id != table::CLOSE_PAREN)
                {
                    recursive_parse(scanner, file, token, cgen, error, scope::ARGS); // Analyze: arguments (recursively)
                }
                cgen << "!args;\n"; // Close arguments: ft name (..')' { .. }

                cgen << "!ft;\n";
            }
            // Sentence: illegal
            else { _error("illegal sentence", error, cgen); }
        }
    }

    // Get next-token in file - (cleaned)
    bool parser::_scan_file(cinfinity::scanner &scanner, cinfinity::file &file, cinfinity::token &token, fs &cgen,
        cinfinity::error &error, bool mode = SCAN_CLEAN)
    {
        while (scanner.scan(file, token))
        {
            // Skip spaces and automatic-semicolons (';')
            if ((token.id == table::SPACE || token.id == table::AUTO_SEMICOLON) && mode == SCAN_CLEAN) { continue; }

            // Token: code
            error.token = token; // Current-token

            // Illegal (token): unfinished-strings ( ".. | '.. )
            if (token.id == table::ILLEGAL_UNFINISHED__STR || token.id == table::ILLEGAL_UNFINISHED__STRCHAR)
            {
                _error("unfinished string", error, cgen);
            }
            // Illegal (token): unfinished-comment ( /*.. )
            else if (token.id == table::ILLEGAL_UNFINISHED__CMT__MULTI_LINE)
            {
                _error("unfinished comment", error, cgen);
            }

            return true;
        }

        return false;
    }

    // New expected-token (error-diagnosis)
    void parser::_error_expected_token(cinfinity::error &error)
    {
        error.next_token_exist      =  NEXT_TOKEN__UNEXIST;
        error.token_helper.charnum += (error.token_helper.val.size()); // Size: 'token' expected-token (error-handler)
        error.token_helper.val      =  error.expected_token.val;
    }

    // Show error (diagnosis) and stop all + close output-file(s)
    void parser::_error(string message, cinfinity::error &error, fs &file)
    {
        file.close(); // Close file (output), before of stop all
        error.err(message);
    }

    // Write in file '.cgen' (formatted: token-data)
    void parser::_token_write(fs &file, cinfinity::token &token)
    {
        string token_val = "";
        
        // Token without-changes
        if (token.id != table::STR)
        {
            token_val = token.val;
        }
        // Replace: ( " ), with: ( \" )
        else {
            for (char c : token.val)
            {
                if (c == '"') { token_val += "\\\""; continue; } // Replace

                // Character Without-changes (in "..")
                token_val += c;
            }
        }

        file << "{i:" << to_string(token.id) << ";v\"" << token_val << "\";l:" << to_string(token.linenum)
             << ";c:" << to_string(token.charnum) << "}\n";
    }

    // Token is name-or-keyword?
    bool parser::_is_name_or_keyword(uint8 id = table::ILLEGAL)
    {
        switch (id)
        {
            // Token: name or keywords
            case table::NAME:           case table::KEYWORD__FT:   case table::KEYWORD__USE:    case table::KEYWORD__AS:
            case table::KEYWORD__CLASS: case table::KEYWORD__VOID: case table::KEYWORD__BOOL:   case table::KEYWORD__CHAR:
            case table::KEYWORD__STR:   case table::KEYWORD__NUM:  case table::KEYWORD__RETURN: case table::KEYWORD__MATCH:
             { return true; }
        }

        return false;
    }
}
