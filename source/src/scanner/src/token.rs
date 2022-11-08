use super::table::Table;

pub struct Token {
    pub u8     id;
    pub String val;
    pub u64    linenum;
    pub u64    charnum;
}

impl Token {
    pub fn new() -> Token {
        Token {
            id:      Table::Illegal as u8,
            val:     String::from(""),
            linenum: 1 as u64,
            charnum: 0 as u64,
        }
    }
    pub fn clone(&self) -> Token {
        Token {
            id:      self.id.clone(),
            val:     self.val.clone(),
            linenum: self.linenum.clone(),
            charnum: self.charnum.clone(),
        }
    }
}
