// Table of tokens
pub enum Table {
    Name,    // var_name
    Num,     // 100
    Str,     // ".."
    Strchar, // '..'

    OpenParen,    // (
    CloseParen,   // )
    OpenKey,      // {
    CloseKey,     // }
    OpenBracket,  // [
    CloseBracket, // ]
    
    Illegal,
}
