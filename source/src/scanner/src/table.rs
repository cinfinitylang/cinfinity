// Table of tokens
pub enum Table {
    // Keywords //
    Name, // var_name

    // Types (values) //
    Num,     // 100
    Str,     // ".."
    Strchar, // '..'

    // Wrappers //
    OpenParen,    // (
    CloseParen,   // )
    OpenKey,      // {
    CloseKey,     // }
    OpenBracket,  // [
    CloseBracket, // ]

    // Symbols //
    Division,       // /
    Semicolon,      // ;
    Multiplication, // *

    // Others //
    AutoSemicolon, // ;
    
    Space,
    CmtOneline,   // //..
    CmtMultiline, // /*..*/

    // Illegals //
    IllegalCmtMultiline, // /*..
    Illegal,
}
