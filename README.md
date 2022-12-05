# The C∞ programming language

<img src="./C∞.png" alt="C∞" height=168 width=168 />

```C
ft main(): void
{
    print("Hello world!".n)
}
```

---

C∞ is a simple programming language that aims for maximum performance with a focus on systems development. Experimenting with a modern-technical language could be rewarding for typical solutions.

* Then, taking into account the properties of the language development approach, we can argue a simple-catalog: [_**approaches**_](./APPROACHES.md)

---

Now by showing a little bit of code you can better understand what syntax is being talked about and better understand the context.
 - Note: the code block uses C syntax highlighting for better readability, since it obviously does not have one (C∞).

```C
// One of the ways to generate the iconic and famous, "Hello world!"
ft main(): void
{
    print("Hello world!".n) // '.n' adds to the end of the string a newline ('\n' | '\r\n': according to the system)
}
```

**View test-script design (for context only):** [C∞ script - design](./main.c∞)
- As you can see from the script, C∞ tries to maintain a mix of modern and vintage design.

---

### I will now explain some of the expected features of the language.

\---------------------------------------

* **Dynamic and static types**

Static types, as in C, Go or any other language, a specific keyword is used for each type, we know that, but also, you can omit it (in C∞) and get a dynamic type, but not only with that you would get a dynamic type in C∞, you need to understand something else. And it is that the values are like variables without name, that possess their type and their state (as: if they were constant or variables). So:

```C
ft main(): void
{
    /*
        'var_name' is of type: 'str' and the value "Hi", the compiler would detect it as type: 'str',
         so the types are 'matched'.
    */
    var_name: str = "Hi" // OK! (str == str) (str (var_name) == str ("Hi"))
    
    // --- //
    
    /*
        'var_name' without keyword of type is by default: 'untype' and the value "Hi" would be detected
         by the compiler as type: 'str', therefore: "they are not of the same type", although the type 'untype'
         can store string types, the compiler says: not in this way (this to avoid unwanted redefinitions,
          by mistake when misspelling the name).
    */
    var_name = "Hi" // Error! (untype != str) (untype (var_name) != str ("Hi"))
    
    // --- //
    
    /*
        Now yes, 'var_name' and the value "Hi" are of the 'same type', they match.
         - '.untype' removes the type automatically detected by the compiler from the value and adds a type: 'untype'.
        
        This means that the 'untype' is dynamic, in the style of languages such as Python, JavaScript, PHP, ...
    */
    var_name = "Hi".untype // OK!    (untype == untype) (untype (var_name) == untype ("Hi"))
    var_name = 100         // Error! (untype != num)    (untype (var_name) != num (100))
    var_name = 100.untype  // Ok!    (untype == untype) (untype (var_name) == untype (100))
}
```

\----------------------------------------

* **The strings are text only**

The escapes are made outside the content of the strings, allowing a text free of escapes and strange things.

```C
// Going back to the main example of: "Hello world!"
ft main(): void
{
    print("Hello world!".n) // The keyword 'n' is an escape from newline '\n' and the '.' a string union
    
    // --- //
    
    /*
        Obtaining: 'text\n"other text"'
        ---
text
"other text"
        ---
    */
    var_name: str = "text".n.q."other text".q // 'Obtimizing' a: "text\n\"other text\"" (in the case of literals)
    
    // --- //
    
    /*
        Character strings ('..') "almost do not require escapes", although they can still be used.
    */
    var_name: char = ''    // Ok! (is a null character: '\0').
    var_name: char = '''   // Ok! (is a single-quote character: ').
    var_name: char = 'C'   // Ok! (is the capital letter C).
    var_name: char = '阿'  // Ok! (is a Chinese symbol).
    var_name: char = 'अ'  // Ok! (is a Hindi symbol).
    var_name: char = 'अ्'  // Wow, is good!. Ok! (it is a hindi grapheme. If grapheme support will also exist,
                            // but it can be configured, if you only want to take: 'ascii' as type 'char'
                            // (var_name: char = 'A'.byte | var_name: char = '阿'.unicode)).
}
```

---

_\- Missing parts will be updated._

---

* Non-profit, free software

---

[_**Discord**_](https://discord.gg/cHvGKJGrpt)
