#pragma once

#include <string>

// Token data
class token_t {
    public: uint_fast8_t id;
    public: std::string  val;
    public: size_t       linenum = 1; size_t charnum = 0;
};
