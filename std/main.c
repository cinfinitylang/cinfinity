use std::c::stdio as cstdio
use std::cpp
use scan::token   as tok

/* Script of C∞ */

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

    ↻ salute as c: char   // Print: "Hello"
    {
        print(c)
    }

    ↺ salute as c: char   // Print: "olleH"
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
