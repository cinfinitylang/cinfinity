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

#define DIR   "!"
#define ARRAY "array"
#define OPEN  "open:"
#define CLOSE "close;"

#define N "\n"

// Syntactic analyzer for 1-file
export namespace cinfinity
{
    parser::parser(cinfinity::scanner &scan, cinfinity::file &f, cinfinity::token &tok, fs &c, cinfinity::error &err)
        : scanner(scan), file(f), token(tok), cgen(c), error(err) {}

    // Analyze 1-file (all)
    void parser::parse()
    {
        _recursive__parse_to_cgen(wrapper::OTHER);
    }

    // Analyze: wrappers in file and write in other-file: '.cgen'
    void parser::_recursive__parse_to_cgen(uint8 wrapper)
    {
        bool exist = false;

        while (_scan_file(SCAN_CLEAN))
        {
            // Exist-content: write '!open:', after '!close;'
            if ((wrapper == wrapper::PAREN || wrapper == wrapper::KEY || wrapper == wrapper::BRACKET) && !exist
                && !(token.id == table::CLOSE_PAREN || token.id == table::CLOSE_KEY || token.id == table::CLOSE_BRACKET))
             { exist = true; cgen << DIR OPEN N; }
            // Change: '[]' → '!array'
            else if (wrapper == wrapper::BRACKET && !exist && token.id == table::CLOSE_BRACKET) { cgen << DIR ARRAY N; }

            // '(' → '( .. )'
            //  ↑ --→ ↑
            if (token.id == table::OPEN_PAREN)
            {
                _recursive__parse_to_cgen(wrapper::PAREN);
                continue;
            }
            // '{' → '{ .. }'
            //  ↑ --→ ↑
            else if (token.id == table::OPEN_KEY)
            {
                _recursive__parse_to_cgen(wrapper::KEY);
                continue;
            }
            // '[' → '[ .. ]'
            //  ↑ --→ ↑
            else if (token.id == table::OPEN_BRACKET)
            {
                _recursive__parse_to_cgen(wrapper::BRACKET);
                continue;
            }
            // ')' → '( .. )'
            //  ↑ -------→ ↑
            else if (wrapper == wrapper::PAREN)
            {
                // Close
                if (token.id == table::CLOSE_PAREN) { if (exist) { cgen << DIR CLOSE N; } return; }
                // ERROR: unfinished
                else if (token.id == table::CLOSE_KEY || token.id == table::CLOSE_BRACKET)
                 { _error("illegal, expected close parenthesis: ')'"); }
            }
            // '}' → '{ .. }'
            //  ↑ -------→ ↑
            else if (wrapper == wrapper::KEY)
            {
                // Close
                if (token.id == table::CLOSE_KEY) { if (exist) { cgen << DIR CLOSE N; } return; }
                // ERROR: unfinished
                else if (token.id == table::CLOSE_PAREN || token.id == table::CLOSE_BRACKET)
                 { _error("illegal, expected close key: '}'"); }
            }
            // ']' → '[ .. ]'
            //  ↑ -------→ ↑
            else if (wrapper == wrapper::BRACKET)
            {
                // Close
                if (token.id == table::CLOSE_BRACKET) { if (exist) { cgen << DIR CLOSE N; } return; }
                // ERROR: unfinished
                else if (token.id == table::CLOSE_PAREN || token.id == table::CLOSE_KEY)
                 { _error("illegal, expected close bracket: ']'"); }
            }

            _token_write(); // Write token in '.cgen'
        }

        // Exist-content: write '!close;'
        if (exist) { cgen << DIR CLOSE N; }

        if (!_scan_file(SCAN_CLEAN))
        {
            // ')' → '( .. )'
            //  ↑ -------→ ↑
            if (wrapper == wrapper::PAREN)
            {
                _error_expected_token(")", table::CLOSE_PAREN);
                _error("expected close parenthesis");
            }
            // '}' → '{ .. }'
            //  ↑ -------→ ↑
            else if (wrapper == wrapper::KEY)
            {
                _error_expected_token("}", table::CLOSE_KEY);
                _error("expected close key");
            }
            // ']' → '[ .. ]'
            //  ↑ -------→ ↑
            else if (wrapper == wrapper::BRACKET)
            {
                _error_expected_token("]", table::CLOSE_BRACKET);
                _error("expected close bracket");
            }
        }
    }

    // Get next-token in file - (cleaned)
    bool parser::_scan_file(bool mode = SCAN_CLEAN)
    {
        while (scanner.scan(file, token))
        {
            // Skip spaces and automatic-semicolons (';')
            if ((token.id == table::SPACE || token.id == table::AUTO_SEMICOLON) && mode == SCAN_CLEAN) { continue; }

            // Token: code
            error.token_helper = token;
            error.token        = token; // Current-token

            // Illegal (token): unfinished-strings ( ".. | '.. )
            if (token.id == table::ILLEGAL_UNFINISHED__STR || token.id == table::ILLEGAL_UNFINISHED__STRCHAR)
            {
                _error("unfinished string");
            }
            // Illegal (token): unfinished-comment ( /*.. )
            else if (token.id == table::ILLEGAL_UNFINISHED__CMT__MULTI_LINE)
            {
                _error("unfinished comment");
            }

            return true;
        }

        return false;
    }

    // New expected-token (error-diagnosis)
    void parser::_error_expected_token(string expected_token, uint8 id)
    {
        error.next_token_exist      =  NEXT_TOKEN__UNEXIST;
        error.token_helper.charnum += (error.token_helper.val.size()); // Size: 'token' expected-token (error-handler)
        error.token_helper.val      = (error.expected_token.val = expected_token);
        error.expected_token.id     =  id;
    }

    // Show error (diagnosis) and stop all + close output-file(s)
    void parser::_error(string message)
    {
        // Close file (output) and remove
        cgen.close();
        remove(path_cgen.c_str());
        
        error.err(message);
    }

    // Write in file '.cgen' (formatted: token-data)
    void parser::_token_write()
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

        cgen << "{i:" << to_string(token.id) << ";v:\"" << token_val << "\";l:" << to_string(token.linenum)
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

    /*
    // Analyze next-sentence in file
    void parser::_recursive__parse_to_cgen(uint8 scope = scope::NESTED)
    {
        // Begin-of-argument(s) of function: 'ft'
        if (token.id == table::NAME)
        {
            GOTO__INITARG:

            string sentence = (scope == scope::ARGS? "argument": "variable"); // Is: argument or variable?

            // IS: name ('name: (..) = value')
            //            ↑--↑ name

            _token_write(); // Token: name

            goto GOTO__GETARG;
        }

        while (_scan_file(SCAN_CLEAN))
        {
            // Sentence: function - ('ft name(..): (..) { .. }')
            if (token.id == table::KEYWORD__FT)
            {
                // ERROR: illegal scope (for 'ft')
                if (scope != scope::GLOBAL) { _error("illegal scope for 'ft'"); }

                cgen << DIR DIR__FT DIR__BEGIN N;

                // IS: name ('ft name(..): (..) { .. }')
                //               ↑--↑ name

                // ERROR: unexist
                if (!_scan_file(SCAN_CLEAN))
                {
                    _error_expected_token("name", table::NAME);
                    _error("expected name for 'ft'");
                }
                // ERROR: illegal
                if (token.id != table::NAME) { _error("illegal name for 'ft'"); }
                _token_write(); // Token: name

                // IS: open-argument(s) ('ft name(..): (..) { .. }')
                //                               ↑ '('

                // ERROR: unexist
                if (!_scan_file(SCAN_CLEAN))
                {
                    _error_expected_token("(", table::OPEN_PAREN);
                    _error("expected open arguments for 'ft'");
                }
                // ERROR: illegal
                if (token.id != table::OPEN_PAREN) { _error("illegal open-argument(s) for 'ft'"); }
                cgen << DIR DIR__ARGS DIR__BEGIN N; // Open arguments: ft name '('..) { .. }

                // IS: close-argument(s) ('ft name(..): (..) { .. }')
                //                                   ↑ ')'

                // ERROR: unexist
                if (!_scan_file(SCAN_CLEAN))
                {
                    _error_expected_token(")", table::CLOSE_PAREN);
                    _error("expected close arguments for 'ft'");
                }
                // ERROR: illegal
                if (token.id != table::CLOSE_PAREN && token.id != table::NAME)
                 { _error("illegal argument name or close arguments for 'ft'"); }

                // Arguments
                if (token.id != table::CLOSE_PAREN)
                {
                    recursive_parse(scope::ARGS); // Analyze: arguments (recursively)
                }
                cgen << DIR DIR__ARGS DIR__END N; // Close arguments: ft name (..')' { .. }

                cgen << DIR DIR__FT DIR__END N;
            }
            // Sentence: argument - ('name: [][]type = value')
            else if (token.id == table::NAME)
            {
                GOTO__GETARG:

                // IS: type-declarer or value-assigner ('name: (..)  = value')
                //                                           ↑ ':' | ↑ '='

                // ERROR: unexist
                if (!_scan_file(SCAN_CLEAN))
                {
                    _error_expected_token(")", table::CLOSE_PAREN);
                    _error("expected close arguments for 'ft'");
                }
                // ERROR: illegal
                if (token.id != table::COLON && token.id != table::EQUAL && token.id != table::COMMA && token.id != table::CLOSE_PAREN)
                {
                    _error("illegal close arguments for 'ft'");
                }
                // Token: ':' | '=' | ','
                if      (token.id == table::COLON)       { cgen << DIR DIR__TYPE DIR__BEGIN N;                          }
                else if (token.id == table::EQUAL)       { cgen << DIR DIR__VAL  DIR__BEGIN N;                          }
                else if (token.id == table::COMMA)       { cgen << DIR DIR__SEP  DIR__BEGIN N; goto GOTO_SEP;           }
                else if (token.id == table::CLOSE_PAREN) { cgen << DIR DIR__TYPE DIR__BEGIN N DIR DIR__TYPE DIR__END N; }

                // IS: type-declarer ('name: (..) = value')
                //                         ↑ type | (type, ..) | ..
                if (token.id == table::COLON)
                {
                    recursive_parse(scope::ARGS); // Analyze: type (recursively)
                    cgen << DIR DIR__TYPE DIR__END N;
                }
                // IS: value-assigner ('name: (..) = value')
                //                                 ↑ ".." | 100, ..
                else if (token.id == table::EQUAL)
                {
                    recursive_parse(scope::ARGS); // Analyze: value (recursively)
                    cgen << DIR DIR__VAL DIR__END N;
                }

                // IS: more arguments ('name: (..) = value, ..')
                //                                        ↑ ','

                if (token.id == table::COMMA) { GOTO_SEP: goto GOTO__INITARG; }
            }
            // Sentence: illegal
            else { _error("illegal sentence"); }
        }
    }
    */
}
