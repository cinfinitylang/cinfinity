// Get next token in file (with mode)
pub fn scanner(file: &mut Script, mode: bool) -> bool {
    while (get_token(file)) {
        return true;
    }
    return false;
}

// Get next token in file (get all tokens)
pub fn get_token(file: &mut Script) -> bool {
    while (file.contains()) {
        print!("[{}]", file.get_char());
        return true;
    }
    return false;
}
