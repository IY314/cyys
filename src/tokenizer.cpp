#include "tokenizer.hpp"

#include <iostream>
#include <regex>

// These rules are ignored
#define RE_COMMENT R"(^#.*?\n|\r$)"
#define RE_SPACE   R"(^\s+)"

#define RE_INT   R"(^\d+)"
#define RE_FLOAT R"(^\d+\.\d+)"

// ONLY SINGLE QUOTES
#define RE_STRING R"(^'.*?')"

// This means any of ( ) [ ] { } , . |, :, =, <, >, ==, !=, <=, >=
#define RE_LIT R"(^(==)|(!=)|(<=)|(>=)|([()\[\]{},.|:=<>]))"

#define RE_KEYWORD  // TODO

// The classic identifier rule (as in C)
#define RE_IDENT R"(^[[:alpha:]]\w*)"

bool match(const std::string &str, const std::string &regex, std::string &res)
{
    // std::cout << "Matching " << str << " against " << regex << '\n';
    std::smatch m;
    bool r = std::regex_search(str, m, std::regex(regex));
    if (r) res = m[0];
    return r;
}

#define matches(rgx) (match(ftext.substr(start.idx), rgx, res))

#define match_add_token(rgx, typ, cnv)                  \
    if (matches(rgx))                                   \
    {                                                   \
        /* std::cout << "found " << res << std::endl;*/ \
        pos += res;                                     \
        tokens.push_back({typ, cnv, start, pos});       \
    }

std::vector<Token> tokenize(const std::string &fname,
                            const std::string &ftext) noexcept
{
    std::vector<Token> tokens;

    Pos pos{0, 1, 0, fname, ftext};

    std::string res;

    while (pos.idx < ftext.length())
    {
        const Pos start(pos);
        if (matches(RE_COMMENT) || matches(RE_SPACE))
            pos += res;
        else
            match_add_token(RE_INT, Token::Type::Int, std::stoll(res))
        else match_add_token(RE_FLOAT, Token::Type::Float, std::stold(res))
        else match_add_token(RE_STRING, Token::Type::String, res)
        // TODO: keyword
        else match_add_token(RE_IDENT, Token::Type::Ident, res)
        else if (matches(RE_LIT))
        {
            Token::LitType type;
            bool found = false;
#define lit_match(l, t) \
    if (res == l) type = t, found = true
            lit_match("(", Token::LitType::LParen);
            else lit_match(")", Token::LitType::RParen);
            else lit_match("[", Token::LitType::LBracket);
            else lit_match("]", Token::LitType::RBracket);
            else lit_match("{", Token::LitType::LBrace);
            else lit_match("}", Token::LitType::RBrace);
            else lit_match(",", Token::LitType::Comma);
            else lit_match(".", Token::LitType::Dot);
            else lit_match("|", Token::LitType::Pipe);
            else lit_match(":", Token::LitType::Colon);
            else lit_match("=", Token::LitType::Assign);
            else lit_match("==", Token::LitType::Equals);
            else lit_match("!=", Token::LitType::NotEquals);
            else lit_match("<", Token::LitType::Less);
            else lit_match(">", Token::LitType::More);
            else lit_match("<=", Token::LitType::LessEquals);
            else lit_match(">=", Token::LitType::MoreEquals);
            if (found) pos += res;
#undef lit_match
        }

        if (pos.idx == start.idx) return {};
    }

    return tokens;
}
