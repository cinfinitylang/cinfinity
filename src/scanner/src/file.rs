pub struct File {
    content: String,

    pub linenum: u64, pub charnum: u64,
}

impl File {
    pub fn open(path: &String) -> File {
        File {
            content: std::fs::read_to_string(path).unwrap(),

            linenum: 1u64, charnum: 0u64,
        }
    }

    // Exist: file content?
    pub fn contains(&self) -> bool { !self.content.is_empty() }

    // See next byte in file (unadvance: file position)
    pub fn see(&self) -> char { self.content.chars().next().unwrap() }

    // Get next byte in file (advance: file position)
    pub fn get(&mut self) -> char {
        let c: char = self.content.remove(0);

        // Advance: file position
        if c != '\n' { self.charnum += 1;                   }
        else         { self.linenum += 1; self.charnum = 0; }

        c
    }
}
