#pragma once

#include <fstream>

// Mode: advance/unadvance file-position
#define FILE_ADVANCE   true
#define FILE_UNADVANCE false

namespace cinfinity
{
    struct file
    {
        using uint     = std::uint_fast64_t;
        using string   = std::string;
        using fs       = std::fstream;
        using openmode = std::ios::openmode;

        uint linenum;
        uint charnum;
        
        file ();
        file (string, openmode);
        ~file();

        void   open       (string, openmode);
        bool   contains   ();
        char   get        ();
        char   see        ();
        string get_unicode();
        string see_unicode();

        // Private //

        private:
        fs     _desc; // Descriptor
        string _get_unicode(char, bool);
    };
}
