use scanner::{
    file::File,
    token::Token,
    table::Table,
    error::Error,
    scanner::scanner,
};

// Compilation: cargo run | cargo build --release (--target 'system')
fn main() {
    let path_file: String = String::from("./std/main.c∞");
    let path_cgen: String = String::from("./std/main.cgen");

    let mut fs = File::open(&path_file);
    let cgen = std::fs::File::create(&path_cgen).unwrap();

    let mut token: Token = Token::new();
    let mut err:   Error = Error::new(); err.path = path_file.clone();
    let mut val: String;

    while scanner(&mut fs, &mut token) {
        val = token.val(Table::Nil as u8, "", &fs);

        if token.id == Table::Space as u8 { continue; }

        err.token = token.clone();
        err.error("illegal");

        print!("[{}]", val);
    }
    /*
    cinfinity::error error;
     INIT_ERROR(error);
     INIT_TOKEN(error.token);
     INIT_TOKEN(error.token_helper);
     INIT_TOKEN(error.expected_token);
     error.path = "./std/main.ci";
     #if defined(OS_WIN)
        error.win_console = CONSOLE;
     #endif
    
    cinfinity::parser parser(scanner, file, token, cgen, error);
    parser.path_cgen = path_cgen;

    // Analyze: C∞-code + get code
    parser.parse();
    
    cgen.close(); // Close: file '.cgen'
    */
}
