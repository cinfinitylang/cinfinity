// Table of tokens
pub enum Table {
    // Keywords //
    Name, // var_name

    KeyFT,     // ft
    KeyUSE,    // use
    KeyAS,     // as
    KeyCLASS,  // class
    KeyVOID,   // void
    KeyBOOL,   // bool
    KeyCHAR,   // char
    KeySTR,    // str
    KeyNUM,    // num
    KeyRETURN, // return
    KeyMATCH,  // match

    // Values //
    Str,     // ".."
    Strchar, // '..'
    Num,     // 10

    // Symbols //
    OpenParen,    // (
    CloseParen,   // )
    OpenKey,      // {
    CloseKey,     // }
    OpenBracket,  // [
    CloseBracket, // ]

    Eq,          // =
    Cycle,       // ↻
    CycleRev,    // ↺
    Div,         // /
    Sem,         // ;
    Colon,       // :
    Dot,         // .
    DoubleDot,   // ..
    Comma,       // ,
    DoubleColon, // ::
    Infinity,    // ∞

    // Others //
    Space,
    Autosem,      // ;
    CmtLine,      // //..
    CmtMultiline, // /*..*/

    // Illegals //
    IllegalUnfinishedCmtMultiline, // /*..
    IllegalUnfinishedStr,          // "..
    IllegalUnfinishedStrchar,      // '..
    Illegal
}
