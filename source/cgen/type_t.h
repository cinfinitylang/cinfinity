#pragma once

#include "./cgen.h"

// 'Dynamic type'
union type_t
{
    cgen_t cgen;
    ft_t   ft;

     type_t(void) {};
    ~type_t(void) {};

    type_t &operator=(type_t& type) { return type; }
};
