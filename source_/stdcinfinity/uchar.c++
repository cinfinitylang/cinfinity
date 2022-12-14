module;

#include <cstdio>
#include <cstdint>
#include <array>

/* Unicode character as 'uchar': number of 4 bytes | Wide character as 'wchar': string of 1 to 8 bytes */

export module uchar;

export typedef   std::uint_fast32_t                uchar;             // uchar as number
export constexpr std::uint_fast8_t                 WCHAR_MAXSIZE = 8; // Maximum size of bytes of wchar
export typedef   std::array<char, WCHAR_MAXSIZE+1> wchar;             // wchar

// Get uchar: from wchar
export uchar wchar_to_uchar(wchar &wchr) noexcept
{
    uchar uchr = 0;
    for (std::size_t i = 0; wchr[i] != '\0'; ++i) { uchr = uchr << 8; uchr += wchr[i]; }
    return uchr;
}

// Get wchar: from uchar
export void uchar_to_wchar(wchar &wchr, uchar uchr) noexcept
{
    char c; std::size_t i = 0;
    for (; uchr > 0; ++i) { c = uchr % 256; wchr[i] = c; uchr = uchr >> 8; } wchr[i] = '\0';
}
