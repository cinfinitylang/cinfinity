use super::script::Script;

// Scan mode's
pub const SCAN_CLEAN:  bool = true;
pub const SCAN_GETALL: bool = false;

// Get next token in file (with mode)
pub fn scan_file(file: &mut Script, mode: bool) -> bool {
    while get_token(file) {
        return true;
    }
    return false;
}

// Get next token in file (get all tokens)
pub fn get_token(file: &mut Script) -> bool {
    while file.contains() {
        print!("[{}]", file.get_char());
        return true;
    }
    return false;
}
