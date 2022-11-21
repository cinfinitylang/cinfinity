#pragma once

#include <iostream>

#include "../stdlib/file.h"
#include "../props/token_t.h"

#include "./table.h"

struct scanner_t
{
    private: cinfinity::file file;
    
    public:
    
    token_t token;

    scanner_t(void) = default;
    scanner_t(std::string path)      { self.open(path);                        }
    void open(std::string path = "") { self.file.open(path, std::fstream::in); }

    // Lexicographic analyzer - (1-file)
    bool scan(void)
    {
        while (get_token())
        {
            // Skip tokens as comments, ..
            if (self.token.id == TABLE__CMT__ONE_LINE || self.token.id == TABLE__CMT__MULTI_LINE)
            {
                continue;
            }
            // Retoken: keywords - (name → keyword)
            else if (self.token.id == TABLE__NAME)
            {
                if      (self.token.value == "ft")     { self.token.id = TABLE__KEYWORD__FT;     }
                else if (self.token.value == "use")    { self.token.id = TABLE__KEYWORD__USE;    }
                else if (self.token.value == "as")     { self.token.id = TABLE__KEYWORD__AS;     }
                else if (self.token.value == "class")  { self.token.id = TABLE__KEYWORD__CLASS;  }
                else if (self.token.value == "match")  { self.token.id = TABLE__KEYWORD__MATCH;  }
                else if (self.token.value == "void")   { self.token.id = TABLE__KEYWORD__VOID;   }
                else if (self.token.value == "bool")   { self.token.id = TABLE__KEYWORD__BOOL;   }
                else if (self.token.value == "char")   { self.token.id = TABLE__KEYWORD__CHAR;   }
                else if (self.token.value == "str")    { self.token.id = TABLE__KEYWORD__STR;    }
                else if (self.token.value == "num")    { self.token.id = TABLE__KEYWORD__NUM;    }
                else if (self.token.value == "return") { self.token.id = TABLE__KEYWORD__RETURN; }
            }

            return true;
        }

        return false;
    }

    // Get next token in file
    bool get_token(void)
    {
        char c;
        while (self.file.contains())
        {
            c = self.file.see();
            switch (c)
            {
                // Token: whitespaces
                break; case ' ': case '\t': case '\r':
                {
                    self.token.id    = TABLE__SPACE; self.file.get();
                    self.token.value = ' ';

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;

                    while (self.file.contains())
                    {
                        c = self.file.see();
                        switch (c)
                        {
                            // Skip united whitespaces
                            break; case ' ': case '\t': case '\r': { self.file.get(); }

                            // Other character
                            break; default: { return true; }
                        }
                    }
                }

                // Token: automatic semicolon ';'
                break; case '\n':
                {
                    self.token.id    = TABLE__AUTO_SEMICOLON; self.file.get();
                    self.token.value = ";";

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;

                    while (self.file.contains())
                    {
                        c = self.file.see();
                        switch (c)
                        {
                            // Skip united newlines
                            break; case '\n': { self.file.get(); }

                            // Other character
                            break; default: { return true; }
                        }
                    }
                }

                // Token: name
                break; case 'a'...'z': case 'A'...'Z': case '_':
                {
                    self.token.id    = TABLE__NAME;
                    self.token.value = self.file.get();

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;

                    while (self.file.contains())
                    {
                        c = self.file.see();
                        switch (c)
                        {
                            // Continuation of name
                            break; case 'a'...'z': case 'A'...'Z': case '0'...'9': case '_': {
                                self.token.value += self.file.get();
                            }

                            // End of name
                            break; default: { return true; }
                        }
                    }
                }

                // Token: number
                break; case '0'...'9':
                {
                    self.token.id    = TABLE__NUM;
                    self.token.value = self.file.get();

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;

                    while (self.file.contains())
                    {
                        c = self.file.see();
                        switch (c)
                        {
                            // Continuation of number
                            break; case '0'...'9': { self.token.value += self.file.get(); }

                            // End of number
                            break; default: { return true; }
                        }
                    }
                }

                // Token: string (double: "..")
                break; case '"':
                {
                    self.token.id    = TABLE__ILLEGAL_UNFINISHED__STR;
                    self.token.value = self.file.get();

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;

                    while (self.file.contains())
                    {
                        c = self.file.see();
                        switch (c)
                        {
                            // End of string
                            break; case '"':
                            {
                                self.token.id     = TABLE__STR;
                                self.token.value += self.file.get();
                                return true;
                            }

                            // Continuation of string
                            break; default: { self.token.value += self.file.get(); }
                        }
                    }
                }

                // Token: string (single: '..')
                break; case '\'':
                {
                    self.token.id    = TABLE__ILLEGAL_UNFINISHED__STRCHAR;
                    self.token.value = self.file.get();

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;

                    // Character as content of string: '..'
                    if (self.file.contains())
                    {
                        self.token.value += ({ c = self.file.get(); c; });

                        // End of empty string (single: '') - (not is: ''')
                        if (c == '\'' && (!self.file.contains() || self.file.see_unicode() != "'"))
                        {
                            self.token.id = TABLE__STRCHAR; return true;
                        }
                    }

                    // End of string ' → '..'
                    //               ↑ ---→ ↑
                    if (self.file.contains() && self.file.see() == '\'')
                    {
                        self.token.id     = TABLE__STRCHAR;
                        self.token.value += self.file.get();
                    }
                }

                // Token: comments (one-line: //.. | multi-line: /*..*/) | division: '/'
                break; case '/':
                {
                    self.token.id    = TABLE__DIVISION;
                    self.token.value = self.file.get();

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;

                    // Is: division '/'
                    if (!self.file.contains()) { return true; }
                    c = self.file.see();

                    // Is: comment (one-line: //..)
                    // / → //..
                    // ↑ -→ ↑
                    if (c == '/')
                    {
                        self.file.get();
                        self.token.id = TABLE__CMT__ONE_LINE;

                        while (self.file.contains())
                        {
                            c = self.file.see();
                            switch (c)
                            {
                                // End of comment (one-line)
                                break; case '\n': { return true; }

                                // Content of comment (one-line)
                                break; default: { self.file.get(); }
                            }
                        }
                    }
                    // Is: comment (multi-line: /*..*/)
                    // * → /*..*/
                    // ↑ -→ ↑
                    else if (c == '*')
                    {
                        self.token.value += self.file.get();
                        self.token.id = TABLE__ILLEGAL_UNFINISHED__CMT__MULTI_LINE;

                        while (self.file.contains())
                        {
                            c = self.file.see();
                            switch (c)
                            {
                                // Possible: end of comment (multi-line)
                                break; case '*':
                                {
                                    self.token.value += self.file.get();

                                    // Yes, is end of comment (multi-line)
                                    if (self.file.contains() && self.file.see() == '/')
                                    {
                                        self.token.id = TABLE__CMT__MULTI_LINE; self.file.get();
                                        return true;
                                    }
                                }

                                // Content of comment (multi-line)
                                break; default: { self.token.value += self.file.get(); }
                            }
                        }
                    }
                }


                // Token: symbols //
                break; case ';': self.token.id = TABLE__SEMICOLON;     goto GOTO__GETCHAR;
                       case '.': self.token.id = TABLE__DOT;           goto GOTO__GETCHAR;
                       case ',': self.token.id = TABLE__COMMA;         goto GOTO__GETCHAR;
                       case '(': self.token.id = TABLE__OPEN_PAREN;    goto GOTO__GETCHAR;
                       case ')': self.token.id = TABLE__CLOSE_PAREN;   goto GOTO__GETCHAR;
                       case '{': self.token.id = TABLE__OPEN_KEY;      goto GOTO__GETCHAR;
                       case '}': self.token.id = TABLE__CLOSE_KEY;     goto GOTO__GETCHAR;
                       case '[': self.token.id = TABLE__OPEN_BRACKET;  goto GOTO__GETCHAR;
                       case ']': self.token.id = TABLE__CLOSE_BRACKET; goto GOTO__GETCHAR;
                       case '=': self.token.id = TABLE__EQUAL;         goto GOTO__GETCHAR;
                {
                    GOTO__GETCHAR:

                    self.token.value = self.file.get();

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;
                }
                break; case ':':
                {
                    self.token.id    = TABLE__COLON;
                    self.token.value = self.file.get();

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;

                    // Is: namespace separator '::' → 'namespace::namespace::name'
                    if (self.file.contains() && self.file.see() == ':')
                    {
                        self.token.value += self.file.get();
                    }
                }

                // Token: illegal
                break; default:
                {
                    self.token.value = self.file.get_unicode();

                    // Is: cycle (normal: spin-alias) - (begin → end)
                    if      (self.token.value == "↻") { self.token.id = TABLE__CYCLE;         }
                    // Is: reverse cycle              - (begin ← end)
                    else if (self.token.value == "↺") { self.token.id = TABLE__CYCLE_REVERSE; }
                    // Is: illegal
                    else                               { self.token.id = TABLE__ILLEGAL;       }

                    self.token.line_number = self.file.line_number;
                    self.token.char_number = self.file.char_number;
                }
            }

            return true;
        }

        return false;
    }
};
