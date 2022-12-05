# C∞ approaches

* **Performance:** I believe that C/C++ being the back-end, you can achieve features that take full advantage of the hardware and speed, with moderate ease and flexibility in their implementation (since C++ has enough robustness and C to have the most access to micro-software. Is not a norm). Leveraging, preprocessing and optimizations in C/C++

* **Security:** Also, I find mechanisms like Rust's to be interesting, but they can also be a bit tiring. So I consider to add or remove these features automatically, as needed, and to put 'unchangeable locks/safety', only when they are really needed (for example: default constant if not modified, variable if modified, explicit constant: if not to be modified 'absolutely'). Also, implement compile-time checkers, such as range, type, pointer, ... or the syntax error diagnostics.

* **Portability:** Modest portability, between systems (not of versions, since priority will be given to the most current standards and tools, due to security issues), thus contributing to the non-native deficiencies of certain systems to others and balancing the balance (as character multi-idiom support).

* **Readability:** A beautiful and simple to write/read design, intuitive and with good usability/utility to advance productivity; And thus focus on the problems or tasks that matter. Like Python, provide broad support and perform tasks in 'modest' but flexible ways.

* **Interoperability:** Because C∞ is a C++ transpiler and intends to remain so, interoperability with C/C++ can be practically straightforward (as if signatures were involved), with only a name space for grouping and protection from unwanted collisions. Adding that C∞ could serve as a kind of 'make', for files that are intended to be object code.

* **Compilation-time:** Maybe 'V' (language), is a good example of fast compilation, at the moment I have only thought about '.C∞' files, that compile to object code only when modified ('make' style) - (initial compilation will obviously be of all files), plus concurrent file compilation and C++ will have power for that.
