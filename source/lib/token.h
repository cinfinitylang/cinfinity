#pragma once

#include <string>

// Initialize: default-values for 'error'-handler
#define INIT_TOKEN(_token_)             \
    (_token_).id      = table::ILLEGAL; \
    (_token_).val     = "";             \
    (_token_).linenum = 1;              \
    (_token_).charnum = 0;

namespace cinfinity
{
    struct token
    {
        using uint8  = std::uint_fast8_t;
        using uint   = std::uint_fast64_t;
        using string = std::string;
        
        uint8  id;
        string val;

        uint linenum;
        uint charnum;
    };
}
