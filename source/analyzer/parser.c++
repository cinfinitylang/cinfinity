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
using string = string;
using fs     = ofstream;

// Syntactic analyzer for 1-file
export namespace cinfinity
{
    // Analyze 1-file (all)
    void parser::parse(cinfinity::scanner &scanner, cinfinity::file &file, cinfinity::token &token, fs &cgen,
        cinfinity::error &error)
    {
        recursive_parse(scanner, file, token, cgen, error, SCOPE__GLOBAL);
    }

    // Analyze next-sentence in file
    void parser::recursive_parse(cinfinity::scanner &scanner, cinfinity::file &file, cinfinity::token &token, fs &cgen,
        cinfinity::error &error, bool scope = SCOPE__NESTED)
    {
        while (_scan_file(scanner, file, token, error))
        {
            // Sentence: 'ft name(..): (..) { .. }'
            if (token.id == table::KEYWORD__FT)
            {
                _get_token_helper(token, error); // Save: 1°-token (helper for error-diagnosis)

                // Is: name ('ft name(..): (..) { .. }')
                //               ↑--↑ name

                // Unexist: name (for 'ft')
                if (!_scan_file(scanner, file, token, error))
                {
                    _error_expected_token("name", error);
                    error.err("expected name for 'ft'");
                }
                // Exist: name (for 'ft')
                if (token.id != table::NAME) { error.err("illegal name for 'ft'"); }

                _token_write(cgen, token);
            }
            // Sentence: illegal
            else { error.err("illegal sentence"); }
        }
    }

    // Get next-token in file - (cleaned)
    bool parser::_scan_file(cinfinity::scanner &scanner, cinfinity::file &file, cinfinity::token &token, cinfinity::error &error)
    {
        while (scanner.scan(file, token))
        {
            // Skip spaces and automatic-semicolons (';')
            if (token.id == table::SPACE || token.id == table::AUTO_SEMICOLON) { continue; }

            // Token: code
            error.token = token;
            return true;
        }

        return false;
    }

    // New expected-token (error-diagnosis)
    void parser::_error_expected_token(string expected_token, cinfinity::error &error)
    {
        error.next_token_exist      = NEXT_TOKEN__UNEXIST;
        error.token_helper.charnum += (error.token_helper.val.size()+1); // '+1' = 1-space: ' ', (error: ft' 'expected-token)
        error.token_helper.val      = expected_token;
    }

    // Backup-token for error-handler
    void parser::_get_token_helper(cinfinity::token &token, cinfinity::error &error) { error.token_helper = token; }

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
}
