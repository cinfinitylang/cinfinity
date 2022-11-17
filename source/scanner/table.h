#pragma once

// Table of tokens
enum
{
    // Keywords //
    TABLE__NAME, // var_name

    TABLE__KEYWORD__FT,     // ft
    TABLE__KEYWORD__USE,    // use
    TABLE__KEYWORD__AS,     // as
    TABLE__KEYWORD__CLASS,  // class
    TABLE__KEYWORD__VOID,   // void
    TABLE__KEYWORD__BOOL,   // bool
    TABLE__KEYWORD__CHAR,   // char
    TABLE__KEYWORD__STR,    // str
    TABLE__KEYWORD__NUM,    // num
    TABLE__KEYWORD__RETURN, // return
    TABLE__KEYWORD__MATCH,  // match

    // Values //
    TABLE__STR,     // ".."
    TABLE__STRCHAR, // '..'
    TABLE__NUM,     // 10

    // Symbols //
    TABLE__OPEN_PAREN,    // (
    TABLE__CLOSE_PAREN,   // )
    TABLE__OPEN_KEY,      // {
    TABLE__CLOSE_KEY,     // }
    TABLE__OPEN_BRACKET,  // [
    TABLE__CLOSE_BRACKET, // ]

    TABLE__EQUAL,         // =
    TABLE__CYCLE,         // ↻
    TABLE__CYCLE_REVERSE, // ↺
    TABLE__DIVISION,      // /
    TABLE__SEMICOLON,     // ;
    TABLE__COLON,         // :
    TABLE__DOT,           // .
    TABLE__COMMA,         // ,
    TABLE__DOUBLE_COLON,  // ::

    // Others //
    TABLE__SPACE,
    TABLE__AUTO_SEMICOLON,  // ;
    TABLE__CMT__ONE_LINE,   // //..
    TABLE__CMT__MULTI_LINE, // /*..*/

    // Illegals //
    TABLE__ILLEGAL_UNFINISHED__CMT__MULTI_LINE, // /*..
    TABLE__ILLEGAL_UNFINISHED__STR,             // "..
    TABLE__ILLEGAL_UNFINISHED__STRCHAR,         // '..
    TABLE__ILLEGAL
};
