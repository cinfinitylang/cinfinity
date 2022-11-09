#pragma once

// Table of tokens
enum
{
    TABLE__NAME,    // var_name
    TABLE__STR,     // ".."
    TABLE__STRCHAR, // '..'
    TABLE__NUM,     // 10

    TABLE__DIVISION,     // /
    TABLE__SEMICOLON,    // ;
    TABLE__COLON,        // :
    TABLE__DOT,          // .
    TABLE__DOUBLE_COLON, // ::

    TABLE__SPACE,
    TABLE__AUTO_SEMICOLON,  // ;
    TABLE__CMT__ONE_LINE,   // //..
    TABLE__CMT__MULTI_LINE, // /*..*/

    TABLE__ILLEGAL_UNFINISHED__CMT__MULTI_LINE, // /*..
    TABLE__ILLEGAL_UNFINISHED__STR,             // "..
    TABLE__ILLEGAL_UNFINISHED__STRCHAR,         // '..
    TABLE__ILLEGAL
};
