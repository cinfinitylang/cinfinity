use super::{
    script::Script,
    token::Token,
    table::Table,
};
use error::{
    err::Error,
};

// Get next token in file (with mode)
pub fn scan_file(file: &mut Script, token: &mut Token, errlang: &mut Error) -> bool {
    
    while get_token(file, token, errlang) {
        // Skip token(s) - (comments, ..)
        if token.id == Table::CmtOneline as u8 ||
           token.id == Table::CmtMultiline as u8 {
            continue;
        }

        return true;
    }
    return false;
}

// Get next token in file (get all tokens)
pub fn get_token(file: &mut Script, token: &mut Token, errlang: &mut Error) -> bool {
    let mut c: char;

    while file.contains() {
        c = file.get_char();
        token.linenum = file.linenum; token.charnum = file.charnum;

        match c {
            // Token: whitespace
            ' ' | '\t' | '\r' => {
                token.id = Table::Space as u8; token.val = String::from(' ');

                while file.contains() {
                    c = file.see_char();

                    match c {
                        // United whitespaces
                        ' ' | '\t' | '\r' => { file.get_char(); },
                        
                        // Other character
                        _ => { return true; },
                    }
                }
            },

            // Token: newline → automatic semicolon: ';'
            '\n' => {
                token.id = Table::AutoSemicolon as u8; token.val = String::from(';');

                while file.contains() {
                    c = file.see_char();

                    match c {
                        // United newlines
                        '\n' => { file.get_char(); },
                        
                        // Other character
                        _ => { return true; },
                    }
                }
            },

            // Token: name
            'a'..='z' | 'A'..='Z' | '_' => {
                token.id = Table::Name as u8; token.val = String::from(c);

                while file.contains() {
                    c = file.see_char();

                    match c {
                        // Continuation of name
                        'a'..='z' | 'A'..='Z' | '0'..='9' | '_' => { token.val.push(file.get_char()); },
                        
                        // End of name
                        _ => { return true; },
                    }
                }
            },

            // Token: number
            '0'..='9' => {
                token.id = Table::Num as u8; token.val = String::from(c);

                while file.contains() {
                    c = file.see_char();

                    match c {
                        // Continuation of number
                        '0'..='9' => { token.val.push(file.get_char()); },
                        
                        // End of number
                        _ => { return true; },
                    }
                }
            },

            // Token: string (double: "..")
            '"' => {
                token.id = Table::Str as u8; token.val = String::from(c);

                while file.contains() {
                    c = file.see_char();

                    match c {
                        // End of string
                        '"' => {
                            token.val.push(file.get_char());
                            return true;
                        },
                        
                        // Continuation of string
                        _ => { token.val.push(file.get_char()); },
                    }
                }

                // Error: unfinished string ("..")
                errlang.abort("unfinished string");
            },

            // Token: string (single: '..')
            '\'' => {
                token.id = Table::IllegalUnfStrchar as u8; token.val = String::from(c);

                // Content (1 character-only: grapheme support) of string ('..')
                if file.contains() { token.val.push(file.get_char()); }

                // End of string: ' → '..'
                //                ↑ ---→ ↑
                if file.contains() && file.see_char() == '\'' {
                    token.id = Table::Strchar as u8;
                    token.val.push(file.get_char());
                }
            },

            // Token: division '/' | comments ('//..' | '/*..*/')
            '/' => {
                token.id = Table::Division as u8; token.val = String::from(c);

                // Is: '/' - (end of file, after of '/')
                if !file.contains() { return true; }

                c = file.see_char();

                // Is: comment (one-line: //..)
                if c == '/' {
                    token.id = Table::CmtOneline as u8;
                    file.get_char();

                    while file.contains() {
                        c = file.see_char();
    
                        match c {
                            // End of comment (one-line)
                            '\n' => { return true; },
                            
                            // Content of comment (one-line)
                            _ => { file.get_char(); },
                        }
                    }

                // Is: comment (multi-line: /*..*/)
                } else if c == '*' {
                    token.id = Table::IllegalUnfCmtMultiline as u8;
                    token.val.push(file.get_char());

                    while file.contains() {
                        c = file.see_char();
    
                        match c {
                            // Possible: end of comment (multi-line) or content: '*' → /*..'*'..*/
                            '*' => {
                                token.val.push(file.get_char());

                                // End of comment (multi-line: '*/')
                                if file.contains() && file.see_char() == '/' {
                                    token.id = Table::CmtMultiline as u8;
                                    file.get_char(); return true;
                                }
                            },
                            
                            // Content of comment (multi-line)
                            _ => { token.val.push(file.get_char()); },
                        }
                    }
                
                // Is: '/'
                } else { return true; }
            },

            // Tokens //

            ':' => {
                token.id = Table::Colon as u8; token.val = String::from(c);

                // Namespace separator: '::' → 'namespace::namespace::property'
                if file.contains() && file.see_char() == ':' {
                    token.id = Table::NamespaceSeparator as u8;
                    token.val.push(file.get_char());
                }
            },
            ';' => { token.id = Table::Semicolon      as u8; token.val = String::from(c); },
            '*' => { token.id = Table::Multiplication as u8; token.val = String::from(c); },
            '.' => { token.id = Table::Dot            as u8; token.val = String::from(c); },
            ',' => { token.id = Table::Comma          as u8; token.val = String::from(c); },

            // Token: illegal
            _ => {
                token.id = Table::Illegal as u8; token.val = String::from(c);
            },
        }

        return true;
    }
    return false;
}
