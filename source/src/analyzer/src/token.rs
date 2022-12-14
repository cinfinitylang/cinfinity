use super::table::Table;

pub struct Token {
    pub id:  u8,
    pub val: String,

    pub linenum: u64, pub charnum: u64,
}

impl Token {
    pub fn new() -> Token {
        Token {
            id:  Table::Illegal as u8,
            val: String::new(),

            linenum: 1u64, charnum: 0u64,
        }
    }

    pub fn clone(&self) -> Token {
        Token {
            id:  self.id.clone(),
            val: self.val.clone(),

            linenum: self.linenum.clone(), charnum: self.charnum.clone(),
        }
    }
}
