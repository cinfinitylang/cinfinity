use scanner::{
    scan_file::{
        scan_file,
    },

    script::Script,
    token::Token,
};

// Compilation: cargo run
fn main() {
    let mut file: Script = Script::new();
    file.read_file(&"../std/main.c∞".to_string());
    let mut token: Token = Token::new();

    while scan_file(&mut file, &mut token) {
        print!("[{}]", token.val);
    }
}
