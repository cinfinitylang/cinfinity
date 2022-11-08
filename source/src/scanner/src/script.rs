pub struct Script {
    String content;

    pub u64    linenum;
    pub u64    charnum;
}

impl Script {
    pub fn new() -> Script {
        Script {
            content: String::from(""),
            linenum: 1 as u64,
            charnum: 0 as u64,
        }
    }
    pub fn clone(&self) -> Script {
        Script {
            content: self.content.clone(),
            linenum: self.linenum.clone(),
            charnum: self.charnum.clone(),
        }
    }

    // Read all content of file
    pub fn read_file(&mut self, path: &String) {
        self.content = fs::read_to_string(path);
    }

    pub fn contains(&self)             { !self.content.is_empty() }
    pub fn get_char(&mut self) -> char { self.content.remove(0)   }
}
