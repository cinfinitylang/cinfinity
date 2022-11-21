#pragma once

#include <fstream>

// Mode: advance/unadvance file-position
#define FILE_ADVANCE   true
#define FILE_UNADVANCE false

namespace cinfinity
{
    struct file
    {
        using openmode = std::ios::openmode;
        using string   = std::string;
        using fs       = std::fstream;
        using uint     = std::uint_fast64_t;

        uint line_number = 1;
        uint char_number = 0;

        file ();
        file (string, openmode);
        ~file();

        void open(string, openmode);

        bool   contains();
        char   get();
        char   see();
        string get_unicode();
        string see_unicode();

        // Private //

        private:
        fs     _desc; // Descriptor
        string _get_unicode(char, bool);
    };
}
