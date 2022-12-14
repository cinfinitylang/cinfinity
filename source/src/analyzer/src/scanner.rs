use super::{
    token::Token,
    table::Table,
    file::File,
};

/* Lexicographic analyzer (1 file) */

// Get next-token in file - (cleaned : lexer)
pub fn scanner(file: &mut File, token: &mut Token) -> bool {
    while get_token(file, token) {
        // Skip tokens as comments, ..
        if token.id == Table::CmtLine as u8 || token.id == Table::CmtMultiline as u8 {
            continue;
        }
        // Retoken: keywords - (name → keyword)
        else if token.id == Table::Name as u8 {
            if      token.val == "ft"     { token.id = Table::KeyFT     as u8; }
            else if token.val == "use"    { token.id = Table::KeyUSE    as u8; }
            else if token.val == "as"     { token.id = Table::KeyAS     as u8; }
            else if token.val == "class"  { token.id = Table::KeyCLASS  as u8; }
            else if token.val == "match"  { token.id = Table::KeyMATCH  as u8; }
            else if token.val == "void"   { token.id = Table::KeyVOID   as u8; }
            else if token.val == "bool"   { token.id = Table::KeyBOOL   as u8; }
            else if token.val == "char"   { token.id = Table::KeyCHAR   as u8; }
            else if token.val == "str"    { token.id = Table::KeySTR    as u8; }
            else if token.val == "num"    { token.id = Table::KeyNUM    as u8; }
            else if token.val == "return" { token.id = Table::KeyRETURN as u8; }
        }

        return true;
    }

    false
}

// Get next token in file
fn get_token(file: &mut File, token: &mut Token) -> bool {
    let mut c: char;
        
    while file.contains() {
        c = file.see();
        match c {

            // Token: space
            ' ' | '\t' | '\r' => {
                token.id  = Table::Space as u8; file.get();
                token.val = String::from(' ');

                token.linenum = file.linenum; token.charnum = file.charnum;

                while file.contains() {
                    match file.see() {
                        // Skip united whitespace(s)
                        ' ' | '\t' | '\r' => { file.get(); },

                        // Other character
                        _ => { return true; },
                    }
                }
            },

            // Token: automatic semicolon ';'
            '\n' => {
                token.id  = Table::Autosem as u8; file.get();
                token.val = String::from(';');

                token.linenum = file.linenum; token.charnum = file.charnum;

                while file.contains() {
                    match file.see() {
                        // Skip united newline(s)
                        '\n' => { file.get(); },

                        // Other character
                        _ => { return true; },
                    }
                }
            },

            // Token: name
            'a'..='z' | 'A'..='Z' | '_' => {
                token.id  = Table::Name as u8;
                token.val = String::from(file.get());

                token.linenum = file.linenum; token.charnum = file.charnum;

                while file.contains() {
                    match file.see() {
                        // Name
                        'a'..='z' | 'A'..='Z' | '0'..='9' | '_' => { token.val.push(file.get()); },

                        // End
                        _ => { return true; },
                    }
                }
            },

            // Token: number
            '0'..='9' => {
                token.id  = Table::Num as u8;
                token.val = String::from(file.get());

                token.linenum = file.linenum; token.charnum = file.charnum;

                while file.contains() {
                    match file.see() {
                        // Number
                        '0'..='9' => { token.val.push(file.get()); },

                        // End
                        _ => { return true; },
                    }
                }
            },

            // Token: string ".."
            '"' => {
                token.id  = Table::IllegalUnfinishedStr as u8;
                token.val = String::from(file.get());

                token.linenum = file.linenum; token.charnum = file.charnum;

                while file.contains() {
                    match file.see() {
                        // End
                        '"' => {
                            token.id = Table::Str as u8;
                            token.val.push(file.get());
                            return true;
                        },

                        // Content
                        _ => { token.val.push(file.get()); },
                    }
                }
            },

            // Token: string '..'
            '\'' => {
                token.id  = Table::IllegalUnfinishedStrchar as u8;
                token.val = String::from(file.get());

                token.linenum = file.linenum; token.charnum = file.charnum;

                // Character: content of '..'
                if file.contains() {
                    c = file.get(); token.val.push(c);

                    // Empty string: '' (not is: ''')
                    if c == '\'' && (!file.contains() || file.see() != '\'') {
                        token.id = Table::Strchar as u8; return true;
                    }
                }

                // End: ' → '..'
                //      ↑ ---→ ↑
                if file.contains() && file.see() == '\'' {
                    token.id = Table::Strchar as u8;
                    token.val.push(file.get());
                }
            },

            // Token: comments ( //.. | /*..*/ ) | division: '/'
            '/' => {
                token.id  = Table::Div as u8;
                token.val = String::from(file.get());

                token.linenum = file.linenum; token.charnum = file.charnum;

                // Division: '/'
                if !file.contains() { return true; }
                c = file.see();

                // Comment: //..
                // / → //..
                // ↑ -→ ↑
                if c == '/' {
                    file.get(); token.id = Table::CmtLine as u8;

                    while file.contains() {
                        match file.see() {
                            '\n' => { return true; }, // End
                            _ => { file.get(); },     // Content
                        }
                    }
                    
                // Comment: /*..*/
                // * → /*..*/
                // ↑ -→ ↑
                } else if c == '*' {
                    token.val.push(file.get());
                    token.id = Table::IllegalUnfinishedCmtMultiline as u8;

                    while file.contains() {
                        match file.see() {
                            // Possible: end?
                            '*' => {
                                token.val.push(file.get());

                                // End
                                if file.contains() && file.see() == '/' {
                                    token.id = Table::CmtMultiline as u8; file.get();
                                    return true;
                                }
                            },

                            // Content
                            _ => { token.val.push(file.get()); },
                        }
                    }
                }
            },


            // Token: symbol(s) //
            ';' | ',' | '=' | '(' | ')' | '{' | '}' | '[' | ']' | '↻' | '↺' | '∞' => {
                match c {
                    ';' => { token.id = Table::Sem        as u8; },
                    ',' => { token.id = Table::Comma      as u8; },
                    '=' => { token.id = Table::Eq         as u8; },
                    '(' => { token.id = Table::OpenParen  as u8; },
                    ')' => { token.id = Table::CloseParen as u8; },
                    '{' => { token.id = Table::OpenParen  as u8; },
                    '}' => { token.id = Table::CloseParen as u8; },
                    '[' => { token.id = Table::OpenParen  as u8; },
                    ']' => { token.id = Table::CloseParen as u8; },
                    '↻' => { token.id = Table::Cycle      as u8; },
                    '↺' => { token.id = Table::CycleRev   as u8; },
                    '∞' => { token.id = Table::Infinity   as u8; },
                    _   => { token.id = Table::Illegal    as u8; },
                }
                token.val = String::from(file.get());

                token.linenum = file.linenum; token.charnum = file.charnum;
            },
            '.' => {
                token.id  = Table::Dot as u8;
                token.val = String::from(file.get());

                token.linenum = file.linenum; token.charnum = file.charnum;

                // Range: .. ('∞'-alias)
                if file.contains() && file.see() == '.' {
                    token.id = Table::DoubleDot as u8;
                    token.val.push(file.get());
                }
            },
            ':' => {
                token.id  = Table::Colon as u8;
                token.val = String::from(file.get());

                token.linenum = file.linenum; token.charnum = file.charnum;

                // Namespace separator: '::' → 'namespace::namespace::name'
                if file.contains() && file.see() == ':' {
                    token.id = Table::DoubleColon as u8;
                    token.val.push(file.get());
                }
            },

            // Token: illegal
            _ => {
                token.val.push(file.get());
                token.id = Table::Illegal as u8;

                token.linenum = file.linenum; token.charnum = file.charnum;
            },
        }

        return true;
    }
    false
}
