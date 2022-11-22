#pragma once

#include "../lib/file.h"
#include "../lib/token.h"

namespace cinfinity
{
    struct scanner
    {
        bool scan     (cinfinity::file&, cinfinity::token&);
        bool get_token(cinfinity::file&, cinfinity::token&);
    };
}
