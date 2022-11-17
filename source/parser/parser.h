#pragma once

#include "../scanner/scanner.h"
#include "../error/error.h"

// Syntactic analyzer (1-file)
struct parser_t
{
    private: scanner_t scanner; error_t error;

    public:

    parser_t(void) = default;
    parser_t(std::string path, std::ios::openmode mode)
    {
        self.scanner.open(path, mode); self.error.path = path;
    }

    // Get code generated of file + analyze syntax
    void parse(void)
    {
        while (self.scanner.scan())
        {
            // Skip spaces and automatic-semicolons (';')
            if (self.scanner.token.id == TABLE__SPACE || self.scanner.token.id == TABLE__AUTO_SEMICOLON)
             { continue; }

            // Sentence: ft name(): type { .. }
            if (self.scanner.token.id == TABLE__KEYWORD__FT)
            {
                //if (self.scanner.token) {}
            }
            // Sentence: illegal
            else
            {
                self._error("illegal sentence");
            }

            /*
            // Skip
            if (self.scanner.token.id == TABLE__SPACE || self.scanner.token.id == TABLE__AUTO_SEMICOLON)
            {
                continue;
            }
            */

            //error.line_number = self.scanner.token.line_number;
            //error.char_number = self.scanner.token.char_number;
            //std::exit(EXIT_FAILURE);
            //std::cout << "[" << self.scanner.token.value << "] ";
        }
    }

    // Private //

    // Throw error (diagnosis/message) and stop all
    private: void _error(std::string message = "")
    {
        self.error.line_number = self.scanner.token.line_number;
        self.error.char_number = self.scanner.token.char_number;

        self.error.error(message);
    }

    // Throw warning (diagnosis/message) and continue
    void _warning(std::string message = "")
    {
        self.error.line_number = self.scanner.token.line_number;
        self.error.char_number = self.scanner.token.char_number;

        self.error.warning(message);
    }
};
