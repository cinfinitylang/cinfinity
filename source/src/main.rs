use scanner::{
    scan_file::{
        scan_file,
    },

    script::Script,
    token::Token,
};
use error::{
    conf_message,

    err::Error,
};

// Compilation: cargo run
fn main() {
    conf_message!(); // Configure error message

    let path: String = "../std/main.c∞".to_string();

    let mut file: Script = Script::new();
     file.read_file(&path);
    let mut token: Token = Token::new();
    let mut errlang: Error = Error::new();
     errlang.path = path.clone();

    while scan_file(&mut file, &mut token, &mut errlang) {
        //print!("[{}]", token.val);
    }
}
