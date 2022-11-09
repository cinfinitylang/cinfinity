#pragma once

#include <fstream>

class file_t
{
    private: std::fstream descriptor;

    public: std::uint_fast64_t line_number = 1;
    public: std::uint_fast64_t char_number = 0;

    public: file_t(void) = default;
    public: file_t(std::string path, std::ios::openmode mode)    { self.open(path, mode); }
    public: void open(std::string path = "", std::ios::openmode mode
        = std::fstream::in | std::fstream::out) { self.descriptor.open(path, mode); }
    public: ~file_t(void) { if (self.descriptor.is_open()) { self.descriptor.close(); } }

    // Check if exist content in file
    public: bool contains(void)
    {
        char c;
        if ((c = (char)self.descriptor.get()) == (char)EOF) { return false; }

        self.descriptor.unget();
        return true;
    }

    // Get next byte in file - (advance file-position)
    public: char get(void)
    {
        char c = (char)self.descriptor.get();

        // Advance file-position
        if (c != '\n') { self.char_number++;                        }
        else           { self.line_number++; self.char_number = 0; }

        return c;
    }

    // See next byte in file - (not advance file-position)
    public: char see(void)
    {
        char c = (char)self.descriptor.get(); self.descriptor.unget();
        return c;
    }
};
