#pragma once

#include <fstream>
#include <clocale>

// Advance file-position?
#define FILE_ADVANCE   true
#define FILE_UNADVANCE false

struct file_t
{
    private: std::fstream descriptor;
    
    public:
    
    std::uint_fast64_t line_number = 1;
    std::uint_fast64_t char_number = 0;

    file_t(void) = default;
    file_t(std::string path, std::ios::openmode mode)    { self.open(path, mode); }
    void open(std::string path = "", std::ios::openmode mode
        = std::fstream::in | std::fstream::out) { self.descriptor.open(path, mode); }
    ~file_t(void) { if (self.descriptor.is_open()) { self.descriptor.close(); } }

    // Check if exist content in file
    bool contains(void)
    {
        char c;
        if ((c = (char)self.descriptor.get()) == (char)EOF) { return false; }

        self.descriptor.unget();
        return true;
    }

    // Get next byte in file - (advance file-position)
    char get(void)
    {
        char c = (char)self.descriptor.get();

        // Advance file-position
        if (c != '\n') { self.char_number++;                       }
        else           { self.line_number++; self.char_number = 0; }

        return c;
    }

    // See next byte in file - (not advance file-position)
    char see(void)
    {
        char c = (char)self.descriptor.get(); self.descriptor.unget();
        return c;
    }

    // Get next character (unicode) in file - (advance file-position)
    std::string get_unicode(void) { return _get_unicode(self.get()); }

    // Get next character (unicode) in file - (not advance file-position)
    std::string see_unicode(void)
    {
        return _get_unicode((char)self.descriptor.get(), FILE_UNADVANCE);
    }
    
    // Get next character (unicode) in file - (advance file-position or not?)
    private: std::string _get_unicode(char c = '\0', bool advance = FILE_ADVANCE)
    {
        // Size of character (unicode)
        std::uint_fast64_t size = 0;
        switch (c)
        {
            break; default:                        { size = 1; }
            break; case (char)0xC2 ... (char)0xDF: { size = 2; }
            break; case (char)0xE0 ... (char)0xEF: { size = 3; }
            break; case (char)0xF0 ... (char)0xF2: { size = 4; }
        }

        // Get character (unicode) //
        std::string        c_unicode = std::string(1, c); // Add: 1°-byte
        std::uint_fast64_t i_get     = 1;
        for (; i_get < size; ++i_get)
        {
            // File contains (bytes of unicode character)
            if (self.contains()) { c_unicode += (char)self.descriptor.get(); }
        }

        // Unget character (unicode) //
        if (advance == FILE_UNADVANCE)
        {
            for (std::uint_fast64_t i_unget = 0; i_unget < i_get; ++i_unget) { self.descriptor.unget(); }
        }

        return c_unicode;
    }
};
