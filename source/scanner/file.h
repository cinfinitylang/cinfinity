#pragma once

// File handler
class file_t {
    private: std::fstream fs; // Descriptor
    public:  size_t       linenum = 1;
    public:  size_t       charnum = 0;

    // Check if is end of file
    public: bool contains(void)
    {
        if (this->fs.eof() || (char)this->fs.get() == (char)EOF) { return false; }

        this->fs.unget();
        return true;
    }

    // Start file handler
    public: void open(std::string path = "", std::ios::openmode mode
        = std::fstream::in | std::fstream::out) {
        
        this->fs.open(path, mode);
    }

    // Get next byte in file (advance file-position)
    public: char get_byte() {
        char c = (char)this->fs.get();

        // Advance file-position
        if (c != '\n') { this->charnum++;                    }
        else           { this->linenum++; this->charnum = 0; }

        return c;
    }

    // Remove file handler
    public: void close(void) { if (this->fs.is_open()) { this->fs.close(); } }
};
