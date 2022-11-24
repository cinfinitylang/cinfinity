#pragma once

#include <functional>

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
        
        void parse          (cinfinity::scanner&, cinfinity::file&, cinfinity::token&, fs&, cinfinity::error&);
        void recursive_parse(cinfinity::scanner&, cinfinity::file&, cinfinity::token&, fs&, cinfinity::error&, uint8);

        private:

        bool _scan_file           (cinfinity::scanner&, cinfinity::file&, cinfinity::token&, fs&, cinfinity::error&, bool);
        void _error               (string, cinfinity::error&, fs&);
        void _error_expected_token(cinfinity::error&);
        void _token_write         (fs&, cinfinity::token&);
        bool _is_name_or_keyword  (uint8);
    };
}
