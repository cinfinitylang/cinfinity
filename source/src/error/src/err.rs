const ERR_IDENT: &str = "    ";

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
        // Line 1: 'error | path/file' //
        let mut fmt_err: String = self.path.clone() + "\n";

        // Line 2: '    2 | error line' //
        fmt_err = format!("{}{}{} | {}\n", fmt_err,
            ERR_IDENT, self.linenum, "$ ft main(): void");

        // Line 3: '↑ 1 | error message' //
        fmt_err = format!("{}{}↑ {} | {}\n", fmt_err,
            ERR_IDENT, self.charnum, msg);

        // Show error (message) and stop all //
        super::error!("{}", fmt_err);
    }
}
