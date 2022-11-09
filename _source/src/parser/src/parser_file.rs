use scanner::{
    scan_file::{
        scan_file,
    },

    script::Script,
    token::Token,
};
use error::err::Error;

// Syntactic analyzer
pub fn parser_file(file: &mut Script, errlang: &mut Error) {
    let mut token: Token = Token::new();
    
    while scan_file(file, &mut token) {
        //print!("[{}]", token.val);
    }

    // Error: unfinished string ("..")
    errlang.linenum = token.linenum;
    errlang.charnum = token.charnum;
    errlang.abort("unfinished string");
}
