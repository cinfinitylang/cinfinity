#pragma once

#include <iostream>

#include "../props/file_t.h"
#include "../props/token_t.h"

#include "./table.h"

struct scanner_t
{
    private: file_t file;
    
    public:
    
    token_t token;

    scanner_t(void) = default;
    scanner_t(std::string path, std::ios::openmode mode) { self.open(path, mode); }
    void open(std::string path = "", std::ios::openmode mode
        = std::fstream::in | std::fstream::out) { self.file.open(path, mode); }

    // Lexicographic analyzer - (1-file)
    bool scan(void)
    {
        while (get_token())
        {
            // Skip tokens as comments, ..
            if (self.token.id == TABLE__CMT__ONE_LINE || self.token.id == TABLE__CMT__MULTI_LINE) { continue; }
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
            self.token.line_number = self.file.line_number;
            self.token.char_number = self.file.char_number;

            switch (c)
            {
                // Token: whitespaces
                break; case ' ': case '\t': case '\r':
                {
                    self.token.id    = TABLE__SPACE; self.file.get();
                    self.token.value = ' ';

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

                    // Character as content of string: '..'
                    if (self.file.contains()) { self.token.value += self.file.get(); }

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
                        self.file.get();
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
                break; case ';': { self.token.id = TABLE__SEMICOLON; self.token.value = self.file.get(); }
                break; case '.': { self.token.id = TABLE__DOT;       self.token.value = self.file.get(); }
                break; case ':':
                {
                    self.token.id    = TABLE__COLON;
                    self.token.value = self.file.get();

                    // Is: namespace separator '::' → 'namespace::namespace::name'
                    if (self.file.contains() && self.file.see() == ':')
                    {
                        self.token.value += self.file.get();
                    }
                }

                // Token: illegal
                break; default:
                {
                    self.token.id    = TABLE__ILLEGAL;
                    self.token.value = self.file.get();
                }
            }

            return true;
        }

        return false;
    }
};
