#pragma once

// Windows (32/64 bits)
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    
    #define OS_WIN
    #define CONSOLE GetStdHandle(STD_ERROR_HANDLE)
#endif

#include "../lib/token.h"

#define NEXT_TOKEN__EXIST   true
#define NEXT_TOKEN__UNEXIST false

// Initialize: default-values for 'error'-handler
#define INIT_ERROR(_error_)          \
    (_error_).path             = ""; \
    (_error_).next_token_exist = NEXT_TOKEN__EXIST;

namespace cinfinity
{
    struct error
    {
        using string = std::string;
        using uint8  = std::uint_fast8_t;

        string           path;
        cinfinity::token token;
        cinfinity::token token_helper;
        bool             next_token_exist;

        #if defined(OS_WIN)
            HANDLE win_console;
        #endif

        void err (string);
        void warn(string);
        
        private:

        void   _problem      (bool, string);
        string _thousands_sep(string);
        void   _error_color  (string, uint8);
    };
}
