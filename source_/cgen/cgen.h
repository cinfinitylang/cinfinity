#pragma once

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

    ft_t(void) = default;

    // Generate C++-code: function ('ft name(..): (..) { .. }')
    void cgen(std::fstream &file_write)
    {
        file_write << self.name.value << "() {}";
    }
};

// Code generator of C++
struct cgen_t
{
    std::vector<ft_t> fts;

    void cgen(std::fstream &file_write)
    {
        // Generate C++-code: functions ('ft name(..): (..) { .. }'), in file
        for (ft_t ft : self.fts) { ft.cgen(file_write); }
    }
};
