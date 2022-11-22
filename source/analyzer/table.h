#pragma once

// Table of tokens
namespace table
{
    enum
    {
        // Keywords

        NAME, // var_name

        KEYWORD__FT,     // ft
        KEYWORD__USE,    // use
        KEYWORD__AS,     // as
        KEYWORD__CLASS,  // class
        KEYWORD__VOID,   // void
        KEYWORD__BOOL,   // bool
        KEYWORD__CHAR,   // char
        KEYWORD__STR,    // str
        KEYWORD__NUM,    // num
        KEYWORD__RETURN, // return
        KEYWORD__MATCH,  // match

        // Values

        STR,     // ".."
        STRCHAR, // '..'
        NUM,     // 10

        // Symbols

        OPEN_PAREN,    // (
        CLOSE_PAREN,   // )
        OPEN_KEY,      // {
        CLOSE_KEY,     // }
        OPEN_BRACKET,  // [
        CLOSE_BRACKET, // ]

        EQUAL,         // =
        CYCLE,         // ↻
        CYCLE_REVERSE, // ↺
        DIVISION,      // /
        SEMICOLON,     // ;
        COLON,         // :
        DOT,           // .
        COMMA,         // ,
        DOUBLE_COLON,  // ::

        // Others

        SPACE,
        AUTO_SEMICOLON,  // ;
        CMT__ONE_LINE,   // //..
        CMT__MULTI_LINE, // /*..*/

        // Illegals
        
        ILLEGAL_UNFINISHED__CMT__MULTI_LINE, // /*..
        ILLEGAL_UNFINISHED__STR,             // "..
        ILLEGAL_UNFINISHED__STRCHAR,         // '..
        ILLEGAL
    };
}
