module;

#include "./table.h"
#include "./scanner.h" // Self

export module scanner;

// Lexicographic analyzer for 1-file
namespace cinfinity
{
    // Get next-token in file - (cleaned : lexer)
    bool scanner::scan(cinfinity::file &file, cinfinity::token &token)
    {
        while (get_token(file, token))
        {
            // Skip tokens as comments, ..
            if (token.id == table::CMT__ONE_LINE || token.id == table::CMT__MULTI_LINE)
            {
                continue;
            }
            // Retoken: keywords - (name → keyword)
            else if (token.id == table::NAME)
            {
                if      (token.val == "ft")     { token.id = table::KEYWORD__FT;     }
                else if (token.val == "use")    { token.id = table::KEYWORD__USE;    }
                else if (token.val == "as")     { token.id = table::KEYWORD__AS;     }
                else if (token.val == "class")  { token.id = table::KEYWORD__CLASS;  }
                else if (token.val == "match")  { token.id = table::KEYWORD__MATCH;  }
                else if (token.val == "void")   { token.id = table::KEYWORD__VOID;   }
                else if (token.val == "bool")   { token.id = table::KEYWORD__BOOL;   }
                else if (token.val == "char")   { token.id = table::KEYWORD__CHAR;   }
                else if (token.val == "str")    { token.id = table::KEYWORD__STR;    }
                else if (token.val == "num")    { token.id = table::KEYWORD__NUM;    }
                else if (token.val == "return") { token.id = table::KEYWORD__RETURN; }
            }

            return true;
        }

        return false;
    }

    // Get next-token in file - (uncleaned : lexer)
    bool scanner::get_token(cinfinity::file &file, cinfinity::token &token)
    {
        char c; while (file.contains())
        {
            c = file.see();
            switch (c)
            {
                // Token: whitespace(s)
                break; case ' ': case '\t': case '\r':
                {
                    token.id  = table::SPACE; file.get();
                    token.val = ' ';

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;

                    while (file.contains())
                    {
                        c = file.see();
                        switch (c)
                        {
                            // Skip united whitespace(s)
                            break; case ' ': case '\t': case '\r': { file.get(); }

                            // Other character
                            break; default: { return true; }
                        }
                    }
                }

                // Token: automatic semicolon ';'
                break; case '\n':
                {
                    token.id  = table::AUTO_SEMICOLON; file.get();
                    token.val = ';';

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;

                    while (file.contains())
                    {
                        c = file.see();
                        switch (c)
                        {
                            // Skip united newlines
                            break; case '\n': { file.get(); }

                            // Other character
                            break; default: { return true; }
                        }
                    }
                }

                // Token: name
                break; case 'a'...'z': case 'A'...'Z': case '_':
                {
                    token.id  = table::NAME;
                    token.val = file.get();

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;

                    while (file.contains())
                    {
                        c = file.see();
                        switch (c)
                        {
                            // Continuation of name
                            break; case 'a'...'z': case 'A'...'Z': case '0'...'9': case '_': { token.val += file.get(); }

                            // End of name
                            break; default: { return true; }
                        }
                    }
                }

                // Token: number
                break; case '0'...'9':
                {
                    token.id  = table::NUM;
                    token.val = file.get();

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;

                    while (file.contains())
                    {
                        c = file.see();
                        switch (c)
                        {
                            // Continuation of number
                            break; case '0'...'9': { token.val += file.get(); }

                            // End of number
                            break; default: { return true; }
                        }
                    }
                }

                // Token: string ("..")
                break; case '"':
                {
                    token.id  = table::ILLEGAL_UNFINISHED__STR;
                    token.val = file.get();

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;

                    while (file.contains())
                    {
                        c = file.see();
                        switch (c)
                        {
                            // End of string
                            break; case '"':
                            {
                                token.id   = table::STR;
                                token.val += file.get();
                                return true;
                            }

                            // Content of string
                            break; default: { token.val += file.get(); }
                        }
                    }
                }

                // Token: string ('..')
                break; case '\'':
                {
                    token.id  = table::ILLEGAL_UNFINISHED__STRCHAR;
                    token.val = file.get();

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;

                    // Character: as content of string: '..'
                    if (file.contains())
                    {
                        token.val += ({ c = file.get(); c; });

                        // End of empty string (single: '') - (not is: ''')
                        if (c == '\'' && (!file.contains() || file.see_unicode() != "'"))
                        {
                            token.id = table::STRCHAR; return true;
                        }
                    }

                    // End of string ' → '..'
                    //               ↑ ---→ ↑
                    if (file.contains() && file.see() == '\'')
                    {
                        token.id   = table::STRCHAR;
                        token.val += file.get();
                    }
                }

                // Token: comments ( //.. | /*..*/ ) | division: '/'
                break; case '/':
                {
                    token.id  = table::DIVISION;
                    token.val = file.get();

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;

                    // Is: division '/'
                    if (!file.contains()) { return true; }
                    c = file.see();

                    // Is: comment : one-line ( //.. )
                    // / → //..
                    // ↑ -→ ↑
                    if (c == '/')
                    {
                        file.get();
                        token.id = table::CMT__ONE_LINE;

                        while (file.contains())
                        {
                            c = file.see();
                            switch (c)
                            {
                                // End of comment ( //.. )
                                break; case '\n': { return true; }

                                // Content of comment ( //.. )
                                break; default: { file.get(); }
                            }
                        }
                    }
                    // Is: comment : multi-line ( /*..*/ )
                    // * → /*..*/
                    // ↑ -→ ↑
                    else if (c == '*')
                    {
                        token.val += file.get();
                        token.id   = table::ILLEGAL_UNFINISHED__CMT__MULTI_LINE;

                        while (file.contains())
                        {
                            c = file.see();
                            switch (c)
                            {
                                // Possible: end of comment ( /*..*/ )?
                                break; case '*':
                                {
                                    token.val += file.get();

                                    // End of comment ( /*..*/ )
                                    if (file.contains() && file.see() == '/')
                                    {
                                        token.id = table::CMT__MULTI_LINE; file.get();
                                        return true;
                                    }
                                }

                                // Content of comment ( /*..*/ )
                                break; default: { token.val += file.get(); }
                            }
                        }
                    }
                }


                // Token: symbol(s) //
                break; case ';': token.id = table::SEMICOLON;     goto GOTO__GETC; // 'GOTO__GETC' : get character (in token-data)
                       case '.': token.id = table::DOT;           goto GOTO__GETC;
                       case ',': token.id = table::COMMA;         goto GOTO__GETC;
                       case '(': token.id = table::OPEN_PAREN;    goto GOTO__GETC;
                       case ')': token.id = table::CLOSE_PAREN;   goto GOTO__GETC;
                       case '{': token.id = table::OPEN_KEY;      goto GOTO__GETC;
                       case '}': token.id = table::CLOSE_KEY;     goto GOTO__GETC;
                       case '[': token.id = table::OPEN_BRACKET;  goto GOTO__GETC;
                       case ']': token.id = table::CLOSE_BRACKET; goto GOTO__GETC;
                       case '=': token.id = table::EQUAL;         goto GOTO__GETC;
                {
                    GOTO__GETC:

                    token.val = file.get();

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;
                }
                break; case ':':
                {
                    token.id  = table::COLON;
                    token.val = file.get();

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;

                    // Is: namespace-separator '::' → 'namespace::namespace::name'
                    if (file.contains() && file.see() == ':')
                    {
                        token.val += file.get();
                    }
                }

                // Token: illegal
                break; default:
                {
                    token.val = file.get_unicode();

                    // Is: cycle (spin-alias) - (begin → end)
                    if (token.val == "↻")
                    {
                        token.id = table::CYCLE;
                    }
                    // Is: reverse-cycle - (begin ← end)
                    else if (token.val == "↺")
                    {
                        token.id = table::CYCLE_REVERSE;
                    }
                    // Is: illegal
                    else { token.id = table::ILLEGAL; }

                    token.linenum = file.linenum;
                    token.charnum = file.charnum;
                }
            }

            return true;
        }

        return false;
    }
}
