module;

#include "./file.h" // Self

export module file;

using namespace std;
using uint     = uint_fast64_t;
using fs       = fstream;
using openmode = ios::openmode;

export namespace cinfinity
{
    file::file () {}
    file::file (string path, openmode mode) { open(path, mode);                       }
    file::~file()                           { if (_desc.is_open()) { _desc.close(); } }

    void file::open(string path = "", openmode mode = fs::in | fs::out)
    {
        _desc.open(path, mode);
    }

    // File-content exist?
    bool file::contains()
    {
        if ((char)_desc.get() == (char)EOF) { return false; }
        return ({ _desc.unget(); true; });
    }

    // See next-byte in file - (not advance file-position)
    char file::see()
    {
        return ({ char c = (char)_desc.get(); _desc.unget(); c; });
    }

    // Get next-byte in file - (advance file-position)
    char file::get()
    {
        char c = (char)_desc.get();

        // Advance file-position
        if (c != '\n') { charnum++;              }
        else           { linenum++; charnum = 0; }

        return c;
    }

    // See next-character (unicode) in file - (not advance file-position)
    string file::see_unicode()
    {
        return _get_unicode((char)_desc.get(), FILE_UNADVANCE);
    }

    // Get next-character (unicode) in file - (advance file-position)
    string file::get_unicode()
    {
        return _get_unicode(get(), FILE_ADVANCE);
    }
    
    // Get next-character (unicode) in file - (advance file-position or not?)
    string file::_get_unicode(char c = '\0', bool advance = FILE_ADVANCE)
    {
        // Size of character
        uint size = 0;
        switch (c)
        {
            break; default:                        { size = 1; }
            break; case (char)0xC2 ... (char)0xDF: { size = 2; }
            break; case (char)0xE0 ... (char)0xEF: { size = 3; }
            break; case (char)0xF0 ... (char)0xFF: { size = 4; }
        }

        // Get character
        string c_unicode = string(1, c); // Add: 1°-byte
        uint   i_get     = 1;
        for (; i_get < size; ++i_get)
        {
            // File contains (bytes of unicode character)
            if (contains()) { c_unicode += (char)_desc.get(); }
        }

        // Unget character
        if (advance == FILE_UNADVANCE)
        {
            for (uint i_unget = 0; i_unget < i_get; ++i_unget) { _desc.unget(); }
        }

        return c_unicode;
    }
}
