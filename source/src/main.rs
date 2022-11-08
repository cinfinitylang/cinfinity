use scanner::{
    scan_file::{
        scan_file,
        SCAN_CLEAN,
    },

    script::Script,
};

fn main() {
    let mut file: Script = Script::new();
    file.read_file(&"../std/main.c∞".to_string());

    while scan_file(&mut file, SCAN_CLEAN) {}
}
