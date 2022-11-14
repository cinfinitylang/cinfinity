#pragma once

// Code generator of C++
struct codegen_t
{
    ;
};

/*
    Code generator of function:
    --
    ft name(arg_name: type = value, ..): (type, ..) { .. }
    ft name(arg_name: type, ..): type { .. }
    ft name() { .. }
*/
struct ft_t
{
    token_t name;
};
