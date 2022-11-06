#pragma once

#include <fstream>
#include <string>

#include "./file.h"
#include "./token.h"

// Lexicographic analyzer
class scanner_t {
    private: file_t  fs;                // File-descriptor
    public:  bool    mode = mode_clean; // Scan mode
    public:  token_t token;

    public: static const bool mode_clean  = true;
    public: static const bool mode_getall = false;

    // Start file handler
    public: void openfile(std::string path = "", std::ios::openmode mode
        = std::fstream::in | std::fstream::out) {

        this->fs.open(path, mode);
    }

    // Remove file handler
    public: void closefile(void) { this->fs.close(); }

    // Get next token in file (with mode)
    public: bool scan(void)
    {
        while (get_token()) {
            return true;
        }

        return false;
    }

    // Get next token in file
    public: bool get_token(void)
    {
        char c;
        while (this->fs.contains()) {
            c = this->fs.get_byte();
            this->token.linenum = this->fs.linenum;
            this->token.charnum = this->fs.charnum;

            switch (c) {
                case 'a'...'z': case 'A'...'Z': case '_': {
                    this->token.id = 0; this->token.val = c;

                    return true;
                }
                default: {
                    this->token.id = 1; this->token.val = c;

                    return true;
                }
            }

            return true;
        }

        return false;
    }
};
