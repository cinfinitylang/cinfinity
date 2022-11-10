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
            error.error("illegal");
            std::cout << "[" << self.scanner.token.value << "] ";
        }
    }
};
