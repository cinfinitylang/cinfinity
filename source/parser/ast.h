#pragma once

#include <vector>

// Table of ast's
enum
{
    AST__ONE_TOKEN,    // 'token_t'
    AST__MULTI_TOKENS, // 'std::vector<token_t>'

    AST__ILLEGAL
};

// Abstract syntax three
struct ast_t
{
    std::uint_fast8_t id = AST__ILLEGAL;

    union value
    {
        token_t              token;
        std::vector<token_t> tokens;
    };
};

/*
    class ast_t
    {
        ast_t content;
    }
    // (10 + (10 * (5 - 200)))
    10 + r
        ↙↘
      10 * r
          ↙↘
         5 - 200
*/
