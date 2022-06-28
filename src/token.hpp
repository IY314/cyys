#pragma once

#include <string>
#include <variant>

#include "pos.hpp"

struct Token
{
    enum struct Type
    {
        Int,
        Float,
        String,
        Ident,
        Keyword,
        Literal,
        End
    } type;

    enum struct LitType
    {
        LParen,
        RParen,
        LBracket,
        RBracket,
        LBrace,
        RBrace,
        Comma,
        Dot,
        Pipe,
        Colon,
        Assign,
        Equals,
        NotEquals,
        Less,
        More,
        LessEquals,
        MoreEquals
    };

    using ValueType =
        std::variant<long long, long double, std::string, LitType>;

    ValueType value;

    Pos start, end;

    Token(const Token &o) noexcept = default;
    ~Token() noexcept = default;
};
