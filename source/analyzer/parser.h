#pragma once

#include "../lib/file.h"
#include "../lib/token.h"
#include "./scanner.h"
#include "./error.h"

// Scope-mode (global | in functions, .. | ..)
namespace scope
{
    enum
    {
        GLOBAL, // 'name: num = 9'
        ARGS,   // ft name('..') {}
        NESTED  // match { '..' }
    };
}

// Wrapper-type: '(' | '[' | '{' | '}' | ']' | ')'
namespace wrapper
{
    enum
    {
        PAREN,   // (
        KEY,     // {
        BRACKET, // [
        
        OTHER, // Other token (not wrapper)

        ILLEGAL
    };
}

// Scan-mode (with/without space(s) or automatic semicolon(s) ';')
#define SCAN_CLEAN   true
#define SCAN_UNCLEAN false

namespace cinfinity
{
    struct parser
    {
        using uint8  = std::uint_fast8_t;
        using string = std::string;
        using fs     = std::ofstream;

        string path_cgen;

        cinfinity::scanner &scanner;
        cinfinity::file    &file;
        cinfinity::token   &token;
        cinfinity::error   &error;
        fs                 &cgen;

        parser(cinfinity::scanner&, cinfinity::file&, cinfinity::token&, fs&, cinfinity::error&);
        
        void parse                    ();
        void _recursive__parse_to_cgen(uint8);

        private:

        bool _scan_file           (bool);
        void _error               (string);
        void _error_expected_token(string, uint8);
        void _token_write         ();
        bool _is_name_or_keyword  (uint8);
    };
}
