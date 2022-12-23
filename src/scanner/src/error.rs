use std::env::consts::OS;

use windows_sys::Win32::{
    Foundation,

    System::Console::{
        CONSOLE_SCREEN_BUFFER_INFO,
        COORD,
        STD_ERROR_HANDLE,
        SMALL_RECT,

        GetStdHandle,
        GetConsoleScreenBufferInfo,
        SetConsoleTextAttribute,
    }
};

use super::{
    file::File,
    token::Token,
    table::Table,
    scanner::scanner,
};

const COLOR_STD:     u8 = 7;
const COLOR_STDHIGH: u8 = 15;
const COLOR_EXPECT:  u8 = 10;
const COLOR_ERR:     u8 = 12;
const COLOR_WARN:    u8 = 14;

pub const NEXTOK_EXIST:   bool = true;
pub const NEXTOK_UNEXIST: bool = false;

// Type of problem (error | warning)
const PROBLEM_ERR:  bool = true;
const PROBLEM_WARN: bool = false;

const PREFIX_ERR:  &str = "error";
const PREFIX_WARN: &str = "warning";

pub struct Error {
    pub path: String,

    pub token:          Token,
    pub token_helper:   Token,
    pub expected_token: Token,

    pub next_token_exist: bool,

    // For windows
    win_console:  Foundation::HANDLE,
}

impl Error {
    pub fn new() -> Error {
        Error {
            path: String::new(),

            token:          Token::new(),
            token_helper:   Token::new(),
            expected_token: Token::new(),

            next_token_exist: true,

            win_console:  unsafe { GetStdHandle(STD_ERROR_HANDLE) },
        }
    }

    // Show error (diagnosis) and stop all
    pub fn error(&mut self, msg: &str) { self.problem(PROBLEM_ERR, msg); panic!(); }

    // Show warning (diagnosis) and continue
    pub fn warning(&mut self, msg: &str) { self.problem(PROBLEM_WARN, msg); }

    // Show problem (error | warning) (format)
    fn problem(&mut self, problem_type: bool, msg: &str) {
        // Unexist: next token
        if self.next_token_exist == NEXTOK_UNEXIST { self.token = self.token_helper.clone(); }

        // Windows
        let mut console_info: CONSOLE_SCREEN_BUFFER_INFO = CONSOLE_SCREEN_BUFFER_INFO{
            dwSize:              COORD{X: 0, Y: 0},
            dwCursorPosition:    COORD{X: 0, Y: 0},
            wAttributes:         0u16,
            srWindow:            SMALL_RECT{Top: 0i16, Left: 0i16, Bottom: 0i16, Right: 0i16},
            dwMaximumWindowSize: COORD{X: 0, Y: 0},
        };
        let mut pos_token: COORD = COORD{X: 0, Y: 0}; let mut pos_pleca: COORD = COORD{X: 0, Y: 0};

        // Thousands separator for line number ('100000000' → '100 000 000')
        let thousandsep_linenum: String = self._thousands_sep(self.token.linenum().to_string());

        let prefix_size:  u64; let mut indent: u64; let mut i: u64 = 0;
        let linenum_size: u64 = (thousandsep_linenum.len()+1) as u64; // '+1' = 1 space: ' ', at the beginning:
                                                                         // error  | path/file
                                                                         // ' '100 | error line
                                                                         //        | ↑ 1 | error message

        // Line[1]: 'error | path/file'

        // Error
        if problem_type == PROBLEM_WARN {
            self._error_color(&PREFIX_WARN.to_string(), COLOR_WARN);
            prefix_size = PREFIX_WARN.len() as u64; // Size: "warning"
        
        // Warning
        } else {
            self._error_color(&PREFIX_ERR.to_string(), COLOR_ERR);
            prefix_size = PREFIX_ERR.len() as u64; // Size: "error"
        }

        // Format: (get pairing number)
        // --
        //  error''| path/file
        //   100' '| error line
        //         | ↑ 1 | error message
        // --
        // 'error'(5)   > '100'(3)    - (example)
        if prefix_size > linenum_size {
            indent = prefix_size - linenum_size;
        
        //        'error'(5)   < '10 000'(6) - (example)
        } else if prefix_size < linenum_size {
            indent = linenum_size - prefix_size;
            for _ in 0..indent { eprint!(" "); }
        }

        self._error_color(&" | ".to_string(),           COLOR_STDHIGH);
        self._error_color(&(self.path.clone() + "\n "), COLOR_STD);

        // Line[2]: ' 10 | error line'

        // Format: (get pairing number)
        // --
        //  error'   '| path/file
        //   10 000' '| error line
        //            | ↑ 1 | error message
        // --
        // 'error'(5)   > '100'(3)    - (example)
        if prefix_size > linenum_size {
            indent = prefix_size - linenum_size;
            for _ in 0..indent { eprint!(" "); }
            
        //      'error'(5)   < '10 000'(6)   - (example)
        } else if prefix_size < linenum_size {
            indent = linenum_size - prefix_size;
        }

        if   problem_type == PROBLEM_WARN { self._error_color(&thousandsep_linenum, COLOR_WARN); } // Warning
        else                              { self._error_color(&thousandsep_linenum, COLOR_ERR);  } // Error

        if OS == "windows" {
            // Get console position (error token)
            unsafe { GetConsoleScreenBufferInfo(self.win_console, &mut console_info); }
            pos_pleca = console_info.dwCursorPosition;
            pos_pleca.X += 1;
        }
        self._error_color(&" | ".to_string(), COLOR_STDHIGH);

        // Print: error line
        self._error_color(&"".to_string(), COLOR_STD);
        let mut err_fs:                 File  = File::open(&self.path);
        let mut err_token:              Token = Token::new();
        let mut skip_first_whitespaces: bool  = true;
        let mut val:                    String;
        while scanner(&mut err_fs, &mut err_token) {
            val = err_token.val(Table::Nil as u8, "", &err_fs);

            //  Other line                                 ||   Skip 1°-whitespace(s) in error line
            if self.token.linenum() != err_token.linenum() || ((err_token.id == Table::Space as u8 && skip_first_whitespaces) ||
               err_token.id == Table::Autosem as u8) { continue; }
            
            // Error line //
            
            // Error token
            if self.token.charnum() == err_token.charnum() {
                // Get: console position (error token)
                if OS == "windows" {
                    unsafe { GetConsoleScreenBufferInfo(self.win_console, &mut console_info); }
                    pos_token    = console_info.dwCursorPosition;
                    pos_token.Y += 1;
                }

                if   problem_type == PROBLEM_WARN { self._error_color(&val, COLOR_WARN); } // Warning
                else                              { self._error_color(&val, COLOR_ERR);   } // Error

                // Reset to standard color
                self._error_color(&"".to_string(), COLOR_STD);
            }
            // Other tokens (in error line)
            else { eprint!("{}", val); }

            // Unexist: next token
            if self.next_token_exist == NEXTOK_UNEXIST && err_token.charnum() + (val.len() as u64) == self.token.charnum() {
                // Between: names/numbers, add: ' ' (example of error with ' ': token' 'expected-token)
                if (self.expected_token.id == Table::Name as u8 || self.expected_token.id == Table::Num as u8) &&
                   (self.expected_token.id == Table::Name as u8 || self.expected_token.id == Table::Num as u8) {
                    self.token.count_charnum(1);
                    self.token.prepend(' '); // 1 space: ' ' (example: token' 'expected-token)
                }

                self._error_color(&val,            COLOR_EXPECT);
                self._error_color(&"".to_string(), COLOR_STD); // Reset to standard color
                break;
            }

            if skip_first_whitespaces { skip_first_whitespaces = false; }
        }
        eprintln!(""); // '\n'

        // Line[3]: ' ..| ..↑ 1 | error message'

        for _ in 0..(pos_pleca.X as u64) { eprint!(" "); i += 1; }
        self._error_color(&"| ".to_string(), COLOR_STDHIGH);
        //                 ↑--↑ -----→ ↓↓ '+2' = i+size("| ")
        i = if (pos_token.X as u64) >= (i+2) { (pos_token.X as u64) - (i+2) } else { self.token.charnum()-1 as u64 };
        for _ in (0..i).rev() { eprint!(" "); } eprint!("^ ");

        // Warning
        if problem_type == PROBLEM_WARN {
            self._error_color(&self._thousands_sep(self.token.charnum().to_string()), COLOR_WARN);
        
        // Error
        } else {
             self._error_color(&self._thousands_sep(self.token.charnum().to_string()), COLOR_ERR);
        }

        self._error_color(&" | ".to_string(),      COLOR_STDHIGH);
        self._error_color(&(msg.to_string() + "\n"), COLOR_STD); // Reset to standard color (console)
    }

    // Print error with color
    fn _error_color(&self, msg: &String, color_code: u8) {
        // Windows (32/64 bits)
        if OS == "windows" {
            unsafe { SetConsoleTextAttribute(self.win_console, color_code as u16); }
        }

        eprint!("{}", msg);
    }

    // Thousands separator: '100 000'
    //                          ↑ space: ' '
    fn _thousands_sep(&self, num_str: String) -> String {
        let mut num_thousandsep: String = String::new();
        let mut thousandsep_i:   u64    = 0u64;

        // Thousands separator for numbers: '100000000' → '100 000 000'
        for c in num_str.chars().rev() {
            thousandsep_i += 1;

            // Add thousands separator: ' '
            if thousandsep_i > 3 {
                num_thousandsep.insert(0, ' '); thousandsep_i = 0;
            }

            // Not changes
            num_thousandsep.insert(0, c);
        }

        num_thousandsep
    }
}
