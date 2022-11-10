#pragma once

#include "../scanner/table.h"

struct token_t
{
    std::uint_fast8_t id    = TABLE__ILLEGAL;
    std::string       value = "";

    std::uint_fast64_t line_number = 1;
    std::uint_fast64_t char_number = 0;
};
