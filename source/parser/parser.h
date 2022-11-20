#pragma once

#include "../scanner/scanner.h"
#include "../error/error.h"
#include "./ast_t.h"
#include "../cgen/type_t.h"

#define SCOPE__GLOBAL true
#define SCOPE__NESTED false

// Syntactic analyzer (1-file)
struct parser_t
{
    private: scanner_t scanner; error_t error;

    public:

    parser_t(void) = default;
    parser_t(std::string path) { self.scanner.open(path); self.error.path = path; }

    // Get code generated of file + analyze syntax
    void parse(std::fstream &file) { recursive_parse(file, SCOPE__GLOBAL); }

    // Recursive analysis of sentences (1-file)
    void recursive_parse(std::fstream &file, bool scope = SCOPE__NESTED)
    {
        while (self._scan_file())
        {
            // Sentence: 'ft name(..): (..) { .. }'
            if (self.scanner.token.id == TABLE__KEYWORD__FT)
            {
                self._get_token_helper(); // Save first token (as helper for error-diagnosis)

                // Is: name ('ft name(..): type { .. }') //
                //               ↑--↑ name

                // No exit: name ('ft')
                if (!self._scan_file())
                {
                    self._error_expected_token("name");
                    self._error("expected name for 'ft'");
                }
                // Exist: name ('ft')
                if (self.scanner.token.id != TABLE__NAME) { self._error("illegal name for 'ft'"); }

                file << "Hello";

                //self._write_token(file);
            }
            // Sentence: illegal
            else { self._error("illegal sentence"); }
        }
    }

    // Private //

    // Write token generated as C++-code
    private: void _write_token(std::fstream &file)
    {
        file << "{i:" << std::to_string(self.scanner.token.id)
            << ";v:\"" << self.scanner.token.value
            << "\";l:" << std::to_string(self.scanner.token.line_number)
            << ";c:" << std::to_string(self.scanner.token.line_number)
            << "}\n";
    }

    // Get next token in file
    bool _scan_file(void)
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
