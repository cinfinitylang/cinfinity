use scanner::{
    file::File,
    token::Token,
    error::Error,
    parser::parser,
};

// Compilation: cargo run | cargo build --release (--target 'system')
fn main() {
    let path_file: String = String::from("./std/main.c∞");
    let path_cgen: String = String::from("./std/main.cgen");

    let mut fs:     File = File::open(&path_file);
    let mut fs_out: std::fs::File = std::fs::File::create(&path_cgen).unwrap();

    let mut token: Token = Token::new();
    let mut err:   Error = Error::new(); err.path = path_file.clone();

    // Analyze: C∞ code
    parser(&mut fs, &mut token, &mut fs_out, &mut err);
}
