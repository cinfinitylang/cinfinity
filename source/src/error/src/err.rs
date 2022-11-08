use scanner::{
    scan_file::scan_file,
    script::Script,
    token::Token,
    table::Table,
};

pub struct Error {
    pub path:    String,
    pub linenum: u64,
    pub charnum: u64,
}

impl Error {
    pub fn new() -> Error {
        Error {
            path:    String::from(""),
            linenum: 1 as u64,
            charnum: 0 as u64,
        }
    }
    pub fn clone(&mut self) -> Error {
        Error {
            path:    self.path.clone(),
            linenum: self.linenum.clone(),
            charnum: self.charnum.clone(),
        }
    }

    // Show error (message / diagnosis) and stop all
    pub fn abort(&self, msg: &str) {
        let mut err_token_pos: u64 = (self.linenum.to_string().len() as u64) +
            self.charnum.clone()+2; // 3 (+2, +1 in charnum-1) = '|| ' → '|10| ..'
                                    //                            ↑↑↑ --→ ↑--↑↑

        // Line 1: 'error → path/file' //
        let mut fmt_err: String = self.path.clone() + "\n";
        
        // Line 2: '|2| error line' //
        fmt_err = format!("{}|{}| ", fmt_err,
            self.linenum);

        let mut err_file: Script = Script::new();
         err_file.read_file(&(self.path));
        let mut err_token: Token = Token::new();
        let mut first_whitespaces:   bool = true;
        let mut first_token:         bool = true;
        let mut err_whitespaces_num: u64  = 0;

        // Get error line in file
        while scan_file(&mut err_file, &mut err_token) {
            // Other lines - (not error line)
            if err_token.linenum != self.linenum { continue; }

            // Is: error line //

            // Skip whitespaces in begin of error line
            if (err_token.id == Table::Space as u8 && first_whitespaces) ||
                err_token.id == Table::AutoSemicolon as u8 { continue; }

            fmt_err.push_str(err_token.val.as_str());
            first_whitespaces = false;

            // Subtract: 'charnum' (error token) - 'charnum' (whitespaces in begin of error line)
            //  = indicator-position: '↑'
            if first_token {
                first_token         = false;
                err_whitespaces_num = if err_token.charnum > 0 { err_token.charnum-1 } else { 0 };
            }
        }

        fmt_err.push('\n');

        // Line 3: '↑ 1: error message' //
        if (err_token_pos-err_whitespaces_num) > 0 { err_token_pos -= err_whitespaces_num; }
        for _ in 1..=err_token_pos { fmt_err.push(' '); } // Add spaces: '    ..'↑ 1: error message

        fmt_err = format!("{}↑ {}: {}\n", fmt_err,
            self.charnum, msg);

        // Show error (message) and stop all //
        super::error!("{}", fmt_err);
    }
}
