// Configure new message for error handler
#[macro_export]
macro_rules! conf_message {
    () => {
        std::panic::set_hook(Box::new(|err_info| {
            const err_prefix: &str = "error";

            // Get error message and show
            if let Some(err) = err_info.payload().downcast_ref::<String>() {
                eprintln!("{} → {}", err_prefix, err);

            // Show default message (if ↑ fails)
            } else {
                eprintln!("{}: unexpected", err_prefix);
            }
        }));
    }
}

// Configure new message for error handler
#[macro_export]
macro_rules! error {
    // Get 'String' for 'panic(String)' → for 'panic_info.payload().downcast_ref::<String>()'
    //  in 'panic::set_hook(..)'
    ($($vals: expr), +) => { panic!("{}", format!($($vals), +)); }
}
