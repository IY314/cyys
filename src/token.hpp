#pragma once

#include <string>
#include <variant>

#include "pos.hpp"

struct Token {
    enum struct Type {
        Int,
        Float,
        String,
        Ident,
        Keyword,
        Literal,
        Newline
    } type;

    using ValueType = std::variant<long long, long double, std::string>;

    ValueType value;

    Pos start, end;

    Token(const Token& o) noexcept = default;
    ~Token() noexcept = default;
};
