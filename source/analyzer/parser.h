#pragma once

#include "../lib/file.h"
#include "../lib/token.h"
#include "./scanner.h"
#include "./error.h"

// Scope-mode (global | in functions, ..)
#define SCOPE__GLOBAL true
#define SCOPE__NESTED false

namespace cinfinity
{
    struct parser
    {
        using string = std::string;
        using fs     = std::ofstream;
        
        void parse          (cinfinity::scanner&, cinfinity::file&, cinfinity::token&, fs&, cinfinity::error&);
        void recursive_parse(cinfinity::scanner&, cinfinity::file&, cinfinity::token&, fs&, cinfinity::error&, bool);

        private:

        bool _scan_file           (cinfinity::scanner&, cinfinity::file&, cinfinity::token&, cinfinity::error&);
        void _get_token_helper    (cinfinity::token&, cinfinity::error&);
        void _error               (string, cinfinity::error&, fs&);
        void _error_expected_token(string, cinfinity::error&);
        void _token_write         (fs&, cinfinity::token&);
    };
}
