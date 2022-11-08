// Table of tokens
pub enum Table {
    // Keywords //
    Name, // var_name

    // Types (values) //
    Num,     // 100
    NumDec,  // 100.0
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
    Colon,          // :
    Semicolon,      // ;
    Multiplication, // *
    Dot,            // .
    Comma,          // ,

    // Others //
    AutoSemicolon,      // ;
    NamespaceSeparator, // ::
    
    Space,
    CmtOneline,   // //..
    CmtMultiline, // /*..*/

    // Illegals //
    IllegalUnfCmtMultiline, // /*..
    IllegalUnfStr,          // "..
    IllegalUnfStrchar,      // '..
    Illegal,
}
