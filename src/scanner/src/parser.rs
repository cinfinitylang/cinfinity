use std::io::prelude::*;

use super::{
    file::File,
    token::Token,
    table::Table,
    error::{
        NEXTOK_UNEXIST,
        Error,
    },
    scanner::scanner,
};

// Scan-mode (with/without space(s) or automatic semicolon(s) ';')
const SCAN_CLEAN:   bool = true;
const SCAN_UNCLEAN: bool = false;

// Wrapper type: '(' | '[' | '{' | '}' | ']' | ')'
enum Wrapper {
    Paren,   // (
    Key,     // {
    Bracket, // [
        
    Other, // Other token (not wrapper)
}

const ARRAY: &str = "vec";
const OPEN:  &str = "{";
const CLOSE: &str = "}";
const N:     &str = "\n";

// Analyze file (syntactic analyzer)
pub fn parser(fs: &mut File, token: &mut Token, fs_out: &mut std::fs::File, err: &mut Error) {
    _recursive_parse2cgen(fs, token, fs_out, err, Wrapper::Other as u8);
}

// Analyze: wrappers in file and write (in other file: '.cgen')
fn _recursive_parse2cgen(fs: &mut File, token: &mut Token, fs_out: &mut std::fs::File, err: &mut Error, wrapper: u8) {
    let mut exist: bool = false;

    while _scan_file(SCAN_CLEAN, fs, token, err) {
        // If exist content, write '{', after '}'
        if (wrapper == Wrapper::Paren as u8 || wrapper == Wrapper::Key as u8 || wrapper == Wrapper::Bracket as u8) && !exist
            && !(token.id == Table::CloseParen as u8 || token.id == Table::CloseKey as u8 || token.id == Table::CloseBracket as u8)
            { exist = true; fs_out.write_all((OPEN.to_string() + N).as_bytes()).unwrap(); }
        
        // Change: '[]' → 'vec'
        else if wrapper == Wrapper::Bracket as u8 && !exist && token.id == Table::CloseBracket as u8
            { fs_out.write_all((ARRAY.to_string() + N).as_bytes()).unwrap(); }

        // '(' → '( .. )'
        //  ↑ --→ ↑
        if token.id == Table::OpenParen as u8 { _recursive_parse2cgen(fs, token, fs_out, err, Wrapper::Paren as u8);
            continue; }
        // '{' → '{ .. }'
        //  ↑ --→ ↑
        else if token.id == Table::OpenKey as u8 { _recursive_parse2cgen(fs, token, fs_out, err, Wrapper::Key as u8);
            continue; }
        // '[' → '[ .. ]'
        //  ↑ --→ ↑
        else if token.id == Table::OpenBracket as u8 { _recursive_parse2cgen(fs, token, fs_out, err, Wrapper::Bracket as u8);
            continue; }
        // ')' → '( .. )'
        //  ↑ -------→ ↑
        else if wrapper == Wrapper::Paren as u8 {
            // Close
            if token.id == Table::CloseParen as u8 { if exist { fs_out.write_all((CLOSE.to_string() + N).as_bytes()).unwrap(); } return; }
            // ERROR: unfinished
            else if token.id == Table::CloseKey as u8 || token.id == Table::CloseBracket as u8
                { _error("illegal, expected close parenthesis: ')'", err); }
        }
        // '}' → '{ .. }'
        //  ↑ -------→ ↑
        else if wrapper == Wrapper::Key as u8 {
            // Close
            if token.id == Table::CloseKey as u8 { if exist { fs_out.write_all((CLOSE.to_string() + N).as_bytes()).unwrap(); } return; }
            // ERROR: unfinished
            else if token.id == Table::CloseParen as u8 || token.id == Table::CloseBracket as u8
                { _error("illegal, expected close key: '}'", err); }
        }
        // ']' → '[ .. ]'
        //  ↑ -------→ ↑
        else if wrapper == Wrapper::Bracket as u8 {
            // Close
            if token.id == Table::CloseBracket as u8 { if exist { fs_out.write_all((CLOSE.to_string() + N).as_bytes()).unwrap(); } return; }
            // ERROR: unfinished
            else if token.id == Table::CloseParen as u8 || token.id == Table::CloseKey as u8
                { _error("illegal, expected close bracket: ']'", err); }
        }

        _token_write(token, fs, fs_out); // Write token in '.cgen'
    }

    // If exist content, exist '{', then: write '}'
    if exist { fs_out.write_all((CLOSE.to_string() + N).as_bytes()).unwrap(); }

    if !_scan_file(SCAN_CLEAN, fs, token, err) {
        // ')' → '( .. )'
        //  ↑ -------→ ↑
        if wrapper == Wrapper::Paren as u8 {
            _error_expected_token(&")".to_string(), Table::CloseParen as u8, &fs, err);
            _error("expected close parenthesis", err);
        }
        // '}' → '{ .. }'
        //  ↑ -------→ ↑
        else if wrapper == Wrapper::Key as u8 {
            _error_expected_token(&"}".to_string(), Table::CloseKey as u8, &fs, err);
            _error("expected close key", err);
        }
        // ']' → '[ .. ]'
        //  ↑ -------→ ↑
        else if wrapper == Wrapper::Bracket as u8 {
            _error_expected_token(&"]".to_string(), Table::CloseBracket as u8, &fs, err);
            _error("expected close bracket", err);
        }
    }
}

// Get next token in file
fn _scan_file(mode: bool, fs: &mut File, token: &mut Token, err: &mut Error) -> bool {
    while scanner(fs, token) {
        // Skip spaces and auto-semicolons ';'
        if (token.id == Table::Space as u8 || token.id == Table::Autosem as u8) && mode == SCAN_CLEAN { continue; }

        // Token: code
        err.token_helper = token.clone();
        err.token        = token.clone(); // Current token

        // Illegal (token): unfinished strings ( ".. | '.. )
        if token.id == Table::IllegalUnfinishedStr as u8 || token.id == Table::IllegalUnfinishedStrchar as u8 {
            _error("unfinished string", err);
        
        // Illegal (token): unfinished comment ( /*.. )
        } else if token.id == Table::IllegalUnfinishedCmtMultiline as u8 {
            _error("unfinished comment", err);
        }

        return true;
    }

    false
}

    // New expected-token (error-diagnosis)
fn _error_expected_token(expected_token: &String, id: u8, fs: &File, err: &mut Error) {
    let tokhelper_len: u64 = err.token_helper.val(Table::Nil as u8, "", &fs).len() as u64;

    err.next_token_exist = NEXTOK_UNEXIST;
    err.token_helper.count_charnum(tokhelper_len); // Size: 'token' expected-token (error-handler)
    err.token_helper.new_val({ err.expected_token.new_val(expected_token); &err.expected_token.val(Table::Nil as u8, "", &fs) });
    err.expected_token.id = id;
}

// Show error and stop all
fn _error(msg: &str, err: &mut Error) {
    // Remove file (output)
    std::fs::remove_file(err.path.as_str()).unwrap();
    
    err.error(msg);
}

// Write in file '.cgen' (formatted: token-data)
fn _token_write(token: &mut Token, fs: &mut File, fs_out: &mut std::fs::File) {
    let mut token_val: String = String::new();
        
    // Token without changes
    if token.id != Table::Str as u8 {
        token_val = token.val(Table::Nil as u8, "", &fs);
    
    // Replace: ", with: \"
    } else {
        for c in token.val(Table::Nil as u8, "", &fs).chars() {
            if c == '"' { token_val.push_str("\\\""); continue; } // Replace

            // Character Without changes in ".."
            token_val.push(c);
        }
    }
    
    fs_out.write_all(format!("i:{}|v:\"{}\"|l:{}|c:{}\n",
        token.id, token_val, token.linenum(), token.charnum()).as_bytes()).unwrap();
}

// Token is: name or keyword?
fn _is_name_or_keyword(id: u8) -> bool {
    // Token: name or keywords
    if  id == Table::Name      as u8 || id == Table::KeyFT     as u8 || id == Table::KeyUSE   as u8 ||
        id == Table::KeyAS     as u8 || id == Table::KeyCLASS  as u8 || id == Table::KeyVOID  as u8 ||
        id == Table::KeyBOOL   as u8 || id == Table::KeyCHAR   as u8 || id == Table::KeySTR   as u8 ||
        id == Table::KeyNUM    as u8 || id == Table::KeyRETURN as u8 || id == Table::KeyMATCH as u8 { return true; }

    false
}

    /*
    // Analyze next-sentence in file
    void parser::_recursive__parse_to_cgen(uint8 scope = scope::NESTED)
    {
        // Begin-of-argument(s) of function: 'ft'
        if (token.id == table::NAME)
        {
            GOTO__INITARG:

            string sentence = (scope == scope::ARGS? "argument": "variable"); // Is: argument or variable?

            // IS: name ('name: (..) = value')
            //            ↑--↑ name

            _token_write(); // Token: name

            goto GOTO__GETARG;
        }

        while (_scan_file(SCAN_CLEAN))
        {
            // Sentence: function - ('ft name(..): (..) { .. }')
            if (token.id == table::KEYWORD__FT)
            {
                // ERROR: illegal scope (for 'ft')
                if (scope != scope::GLOBAL) { _error("illegal scope for 'ft'"); }

                cgen << DIR DIR__FT DIR__BEGIN N;

                // IS: name ('ft name(..): (..) { .. }')
                //               ↑--↑ name

                // ERROR: unexist
                if (!_scan_file(SCAN_CLEAN))
                {
                    _error_expected_token("name", table::NAME);
                    _error("expected name for 'ft'");
                }
                // ERROR: illegal
                if (token.id != table::NAME) { _error("illegal name for 'ft'"); }
                _token_write(); // Token: name

                // IS: open-argument(s) ('ft name(..): (..) { .. }')
                //                               ↑ '('

                // ERROR: unexist
                if (!_scan_file(SCAN_CLEAN))
                {
                    _error_expected_token("(", table::OPEN_PAREN);
                    _error("expected open arguments for 'ft'");
                }
                // ERROR: illegal
                if (token.id != table::OPEN_PAREN) { _error("illegal open-argument(s) for 'ft'"); }
                cgen << DIR DIR__ARGS DIR__BEGIN N; // Open arguments: ft name '('..) { .. }

                // IS: close-argument(s) ('ft name(..): (..) { .. }')
                //                                   ↑ ')'

                // ERROR: unexist
                if (!_scan_file(SCAN_CLEAN))
                {
                    _error_expected_token(")", table::CLOSE_PAREN);
                    _error("expected close arguments for 'ft'");
                }
                // ERROR: illegal
                if (token.id != table::CLOSE_PAREN && token.id != table::NAME)
                 { _error("illegal argument name or close arguments for 'ft'"); }

                // Arguments
                if (token.id != table::CLOSE_PAREN)
                {
                    recursive_parse(scope::ARGS); // Analyze: arguments (recursively)
                }
                cgen << DIR DIR__ARGS DIR__END N; // Close arguments: ft name (..')' { .. }

                cgen << DIR DIR__FT DIR__END N;
            }
            // Sentence: argument - ('name: [][]type = value')
            else if (token.id == table::NAME)
            {
                GOTO__GETARG:

                // IS: type-declarer or value-assigner ('name: (..)  = value')
                //                                           ↑ ':' | ↑ '='

                // ERROR: unexist
                if (!_scan_file(SCAN_CLEAN))
                {
                    _error_expected_token(")", table::CLOSE_PAREN);
                    _error("expected close arguments for 'ft'");
                }
                // ERROR: illegal
                if (token.id != table::COLON && token.id != table::EQUAL && token.id != table::COMMA && token.id != table::CLOSE_PAREN)
                {
                    _error("illegal close arguments for 'ft'");
                }
                // Token: ':' | '=' | ','
                if      (token.id == table::COLON)       { cgen << DIR DIR__TYPE DIR__BEGIN N;                          }
                else if (token.id == table::EQUAL)       { cgen << DIR DIR__VAL  DIR__BEGIN N;                          }
                else if (token.id == table::COMMA)       { cgen << DIR DIR__SEP  DIR__BEGIN N; goto GOTO_SEP;           }
                else if (token.id == table::CLOSE_PAREN) { cgen << DIR DIR__TYPE DIR__BEGIN N DIR DIR__TYPE DIR__END N; }

                // IS: type-declarer ('name: (..) = value')
                //                         ↑ type | (type, ..) | ..
                if (token.id == table::COLON)
                {
                    recursive_parse(scope::ARGS); // Analyze: type (recursively)
                    cgen << DIR DIR__TYPE DIR__END N;
                }
                // IS: value-assigner ('name: (..) = value')
                //                                 ↑ ".." | 100, ..
                else if (token.id == table::EQUAL)
                {
                    recursive_parse(scope::ARGS); // Analyze: value (recursively)
                    cgen << DIR DIR__VAL DIR__END N;
                }

                // IS: more arguments ('name: (..) = value, ..')
                //                                        ↑ ','

                if (token.id == table::COMMA) { GOTO_SEP: goto GOTO__INITARG; }
            }
            // Sentence: illegal
            else { _error("illegal sentence"); }
        }
    }
*/
