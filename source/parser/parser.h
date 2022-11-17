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
        while (self._scan_file())
        {
            // Sentence: ft name(): type { .. }
            if (self.scanner.token.id == TABLE__KEYWORD__FT)
            {
                self._get_token_helper(); // Save first token (as helper for error-diagnosis)

                if (!self._scan_file())
                {
                    self._error_expected_token("name");
                    self._error("expected name for 'ft'");
                }
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

    // Get next token in file
    private: bool _scan_file(void)
    {
        while (self.scanner.scan())
        {
            // Skip spaces and automatic-semicolons (';')
            if (self.scanner.token.id == TABLE__SPACE || self.scanner.token.id == TABLE__AUTO_SEMICOLON)
             { continue; }

            // Token: code
            self.error.token = self.scanner.token;
            return true;
        }

        return false;
    }

    // Configurate new expected-token for error-diagnosis
    void _error_expected_token(std::string expected_token = "")
    {
        self.error.next_token_exist = NEXT_TOKEN__UNEXIST;
        self.error.token_helper.char_number += (self.error.token_helper.value.size()+1); // '+1' = +1 space: ' ',
                                                                // (example (error-diagnosis): ft' 'expected-token)
        self.error.token_helper.value = expected_token;
    }

    // Save backup token
    void _get_token_helper(void) { self.error.token_helper = self.scanner.token; }
    
    // Throw error (diagnosis/message) and stop all
    void _error(std::string message = "") { self.error.error(message); }

    // Throw warning (diagnosis/message) and continue
    void _warning(std::string message = "") { self.error.warning(message); }
};
