#pragma once

#include <string>

struct Pos
{
    unsigned long long idx, ln, col;
    std::string fname, ftext;

    Pos(const Pos &o) noexcept = default;
    ~Pos() noexcept = default;

    Pos &operator=(const Pos &o) noexcept = default;

    void operator+=(const std::string &passed) noexcept;
};
