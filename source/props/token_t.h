#pragma once

#include "../scanner/table.h"

class token_t
{
    public: std::uint_fast8_t id    = TABLE__ILLEGAL;
    public: std::string       value = "";

    public: std::uint_fast64_t line_number = 1;
    public: std::uint_fast64_t char_number = 0;
};
