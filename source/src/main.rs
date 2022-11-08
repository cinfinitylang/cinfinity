use scanner::{
    scanner,
};

fn main() {
    let mut file: Script = Script::new();

    while scanner(&mut file) {}
}
