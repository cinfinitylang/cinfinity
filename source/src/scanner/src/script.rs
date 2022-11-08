use std::fs;

pub struct Script {
    content: String,

    pub linenum: u64,
    pub charnum: u64,
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
        self.content = fs::read_to_string(path).unwrap();
    }

    // Advance file-position
    pub fn get_char(&mut self) -> char {
        let c: char = self.content.remove(0);

        // Advance file-position
        if c != '\n' { self.charnum += 1;                   }
         else        { self.linenum += 1; self.charnum = 0; }

        return c;
    }
    pub fn see_char(&mut self) -> char { self.content.chars().next().unwrap() } // Not advance file-position
    pub fn contains(&self)     -> bool { !self.content.is_empty()             }
}
