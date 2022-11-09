use scanner::{
    script::Script,
};
use parser::{
    parser_file::parser_file,
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
    let mut errlang: Error = Error::new();
     errlang.path = path.clone();

    parser_file(&mut file, &mut errlang);
}
