use super::{
    file::File,
    table::Table,
};

pub struct Token {
    pub id: u8,
    _val:   String,

    _linenum: u64, _charnum: u64,
}

impl Token {
    pub fn new() -> Token {
        Token {
            id:   Table::Illegal as u8,
            _val: String::new(),

            _linenum: 1u64, _charnum: 0u64,
        }
    }
    
    pub fn clone(&self) -> Token {
        Token {
            id:   self.id.clone(),
            _val: self._val.clone(),
            
            _linenum: self._linenum.clone(), _charnum: self._charnum.clone(),
        }
    }

    // Get/assign value
    pub fn val(&mut self, id: u8, val: &str, fs: &File) -> String {

        // Get/assign or get-only?
        if id != Table::Nil as u8 {
            self.id       = id;
            self._val     = String::from(val);
            self._linenum = fs.linenum; self._charnum = fs.charnum;
        }

        self._val.clone()
    }

    // Assign only-value
    pub fn new_val(&mut self, val: &String) { self._val = val.clone(); }

    // Add at the end to value
    pub fn push(&mut self, c: char) { self._val.push(c); }

    // Add at the begin to value
    pub fn prepend(&mut self, c: char) { self._val.insert(0, c); }

    // Get line number
    pub const fn linenum(&self) -> u64 { self._linenum }

    // Get unicode character number
    pub const fn charnum(&self) -> u64 { self._charnum }

    // Count character number
    pub fn count_charnum(&mut self, add: u64) { self._charnum += add; }
}
