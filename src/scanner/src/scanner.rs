use super::{
    file::File,
    table::Table,
    token::Token,
};

/* Lexicographic analyzer (1 file) */

// Get next token in file (cleaned)
pub fn scanner(fs: &mut File, token: &mut Token) -> bool {
    let val: String = token.val(Table::Nil as u8, "", &fs);

    while get_token(fs, token) {

        // Skip comments, ..
        if token.id == Table::CmtLine as u8 || token.id == Table::CmtMultiline as u8 {
            continue;
        
        // Retoken: keywords (name → keyword)
        } else if token.id == Table::Name as u8 {
            if      val == "ft"     { token.id = Table::KeyFT     as u8; }
            else if val == "use"    { token.id = Table::KeyUSE    as u8; }
            else if val == "as"     { token.id = Table::KeyAS     as u8; }
            else if val == "class"  { token.id = Table::KeyCLASS  as u8; }
            else if val == "match"  { token.id = Table::KeyMATCH  as u8; }
            else if val == "void"   { token.id = Table::KeyVOID   as u8; }
            else if val == "bool"   { token.id = Table::KeyBOOL   as u8; }
            else if val == "char"   { token.id = Table::KeyCHAR   as u8; }
            else if val == "str"    { token.id = Table::KeySTR    as u8; }
            else if val == "num"    { token.id = Table::KeyNUM    as u8; }
            else if val == "return" { token.id = Table::KeyRETURN as u8; }
        }

        return true;
    }

    false
}

// Get next token in file
fn get_token(fs: &mut File, token: &mut Token) -> bool {
    let mut c: char;
        
    while fs.contains() {
        c = fs.see();
        match c {

            // Token: space
            ' ' | '\t' | '\r' => { fs.get();
                token.val(Table::Space as u8, " ", &fs);

                while fs.contains() {
                    match fs.see() {
                        ' ' | '\t' | '\r' => { fs.get();    }, // Skip united whitespace(s)
                        _                 => { return true; }, // Other character
                    }
                }
            },

            // Token: automatic semicolon ';'
            '\n' => { fs.get();
                token.val(Table::Autosem as u8, ";", &fs);

                while fs.contains() {
                    match fs.see() {
                        '\n' => { fs.get();    }, // Skip united newline(s)
                        _    => { return true; }, // Other character
                    }
                }
            },

            // Token: name
            'a'..='z' | 'A'..='Z' | '_' => {
                token.val(Table::Name as u8, fs.get().to_string().as_str(), &fs);

                while fs.contains() {
                    match fs.see() {
                        'a'..='z' | 'A'..='Z' | '0'..='9' | '_' => { token.push(fs.get()); }, // Name
                        _                                       => { return true;          }, // End
                    }
                }
            },

            // Token: number
            '0'..='9' => {
                token.val(Table::Num as u8, fs.get().to_string().as_str(), &fs);

                while fs.contains() {
                    match fs.see() {
                        '0'..='9' => { token.push(fs.get()); }, // Number
                        _         => { return true;          }, // End
                    }
                }
            },

            // Token: string ".."
            '"' => {
                token.val(Table::IllegalUnfinishedStr as u8, fs.get().to_string().as_str(), &fs);

                while fs.contains() {
                    match fs.see() {
                        // End
                        '"' => {
                            token.id = Table::Str as u8;
                            token.push(fs.get());
                            return true;
                        },

                        // Content
                        _ => { token.push(fs.get()); },
                    }
                }
            },

            // Token: string '..'
            '\'' => {
                token.val(Table::IllegalUnfinishedStrchar as u8, fs.get().to_string().as_str(), &fs);

                // Character: content of '..'
                if fs.contains() {
                    c = fs.get(); token.push(c);

                    // Empty string: '' (not is: ''')
                    if c == '\'' && (!fs.contains() || fs.see() != '\'') {
                        token.id = Table::Strchar as u8; return true;
                    }
                }

                // End: ' → '..'
                //      ↑ ---→ ↑
                if fs.contains() && fs.see() == '\'' {
                    token.id = Table::Strchar as u8;
                    token.push(fs.get());
                }
            },

            // Token: comments ( //.. | /*..*/ ) | division: '/'
            '/' => {
                token.val(Table::Div as u8, fs.get().to_string().as_str(), &fs);

                // Division: '/'
                if !fs.contains() { return true; }
                c = fs.see();

                // Comment: //..
                // / → //..
                // ↑ -→ ↑
                if c == '/' {
                    fs.get(); token.id = Table::CmtLine as u8;

                    while fs.contains() {
                        match fs.see() {
                            '\n' => { return true; }, // End
                            _    => { fs.get();    }, // Content
                        }
                    }
                    
                // Comment: /*..*/
                // * → /*..*/
                // ↑ -→ ↑
                } else if c == '*' {
                    token.id = Table::IllegalUnfinishedCmtMultiline as u8; token.push(fs.get());

                    while fs.contains() {
                        match fs.see() {
                            // Possible: end?
                            '*' => {
                                token.push(fs.get());

                                // End
                                if fs.contains() && fs.see() == '/' {
                                    token.id = Table::CmtMultiline as u8; fs.get();
                                    return true;
                                }
                            },

                            // Content
                            _ => { token.push(fs.get()); },
                        }
                    }
                }
            },


            // Token: symbol(s) //
            '(' => { token.val(Table::OpenParen    as u8, fs.get().to_string().as_str(), &fs); },
            ')' => { token.val(Table::CloseParen   as u8, fs.get().to_string().as_str(), &fs); },
            '{' => { token.val(Table::OpenKey      as u8, fs.get().to_string().as_str(), &fs); },
            '}' => { token.val(Table::CloseKey     as u8, fs.get().to_string().as_str(), &fs); },
            '[' => { token.val(Table::OpenBracket  as u8, fs.get().to_string().as_str(), &fs); },
            ']' => { token.val(Table::CloseBracket as u8, fs.get().to_string().as_str(), &fs); },
            ';' => { token.val(Table::Sem          as u8, fs.get().to_string().as_str(), &fs); },
            ',' => { token.val(Table::Comma        as u8, fs.get().to_string().as_str(), &fs); },
            '=' => { token.val(Table::Eq           as u8, fs.get().to_string().as_str(), &fs); },

            '↻' => { token.val(Table::Cycle    as u8, fs.get().to_string().as_str(), &fs); },
            '↺' => { token.val(Table::CycleRev as u8, fs.get().to_string().as_str(), &fs); },
            '∞' => { token.val(Table::Infinity  as u8, fs.get().to_string().as_str(), &fs); },

            '.' => {
                token.val(Table::Dot as u8, fs.get().to_string().as_str(), &fs);

                // Range: .. ('∞'-alias)
                if fs.contains() && fs.see() == '.' {
                    token.id = Table::DoubleDot as u8;
                    token.push(fs.get());
                }
            },

            ':' => {
                token.val(Table::Colon as u8, fs.get().to_string().as_str(), &fs);

                // Namespace separator: '::' → 'namespace::namespace::name'
                if fs.contains() && fs.see() == ':' {
                    token.id = Table::DoubleColon as u8;
                    token.push(fs.get());
                }
            },

            // Token: illegal
            _ => { token.val(Table::Illegal as u8, fs.get().to_string().as_str(), &fs); },
        }

        return true;
    }
    false
}
