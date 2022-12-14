#pragma once

#include "../cgen/cgen.h"

enum
{
    AST__FT, // ft name(..): (..) { .. }

    AST__ILLEGAL
};

struct ast_t
{
    std::uint_fast8_t type = AST__ILLEGAL;

    ft_t ft;
};
