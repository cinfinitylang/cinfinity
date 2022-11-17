// Test main function
ft /*main()
{
    print("Hello world!".n)
}

use std::c::stdio as cstdio
use std::cpp
use scan::token   as tok

/*
    Functions and variables (globals): public by default   - (private-only to the needs)
    Numbers (values):                  positive by default - (with signed-only to the needs: (with: 100.sign))
                                                              100.bytes(1..4)  {1: 8-bit,  2: 16-bit, ..}
                                                              100.0.bytes(4|8) {1: 32-bit, 2: 64-bit}
    Variables (alls):                  mutable by default  - (automatic const in variables-mutables +
                                                               immutable-only to the needs (
                                                                not changed by another programmer
                                                               )
                                                              )
* /

class regex
{                    // ↓ optional (assignment), default initialized types
    ft match(src: str = "", expr: str): bool
    {
        return std::regex::match(src, expr); // ';' optional, at the end of line or file
    }
}

// 'main' function (start program)
ft main(): void
{
    salute: str = "Hello"

    printf("%s", salute)     // Print: C   'std::c::stdio'
    iostream::cout(" world") // Print: C++ 'std::cpp::iostream'
    print("!".n)             // Print: C∞  'print'

    spin salute as c: char // Print: "Hello"
    {
        print(c)
    }

    c: char ↻ salute   // Print: "Hello"
    {
        print(c)
    }

    c: char ↺ salute   // Print: "olleH"
    {
        print(c)
    }

    spin 100 // Print 100 times: "Hi\n"
    {
        print("Hi".n)
    }

    c: char = 'C'
    match c
    {
        'A' .. 'Z'
        {
            print("Letter of alphabet".n)
        }

        // Others (default)
        ..
        {
            print("Unexpected".n)
        }
    }
}
*/