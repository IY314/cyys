#include "tokenizer.hpp"

#include <algorithm>
#include <iostream>
#include <regex>

// These rules are ignored
#define RE_COMMENT R"(^#.*?\n|\r$)"
#define RE_SPACE R"(^\s+)"

// Number rules
#define RE_INT R"(^\d+)"
#define RE_FLOAT R"(^\d+\.\d+)"

// ONLY SINGLE QUOTES
#define RE_STRING "^'.*?'"

// This means any of ( ) [ ] { } , . |, :, =, <, >, ==, !=, <=, >=
#define RE_LIT R"(^(==)|(!=)|(<=)|(>=)|([()\[\]{},.|:=<>]))"

// The classic identifier rule (as in C)
#define RE_IDENT R"(^[[:alpha:]]\w*)"

bool match(const std::string& str, const std::string& regex,
           std::string& res) noexcept {
    // std::cout << "Matching " << str << " against " << regex << '\n';
    std::smatch m;
    bool r = std::regex_search(str, m, std::regex(regex));
    if (r) res = m[0];
    return r;
}

class match_functor {
    const std::string& str;
    std::string& res;

public:
    match_functor(const std::string& str, std::string& res) noexcept
        : str(str), res(res) {}

    bool operator()(const std::string& rgx) noexcept {
        return match(str, rgx, res);
    }
};

std::vector<Token> tokenize(const std::string& fname,
                            const std::string& ftext) noexcept {
#define matches(rgx) (match(ftext.substr(start.idx), rgx, res))

#define match_add_token(rgx, typ, cnv)                       \
    if (matches(rgx)) {                                      \
        pos += res;                                          \
        std::cout << "Found " #typ << ": '" << res << "'\n"; \
        tokens.push_back({typ, cnv, start, pos});            \
    }
    const auto keywords = std::vector<std::string>{
        "^require", "^struct", "^is",  "^end", "^func", "^self",
        "^to",      "^proc",   "^let", "^if",  "^do"};

    std::vector<Token> tokens;

    Pos pos{0, 1, 0, fname, ftext};

    std::string res;

    std::vector<std::string>::const_iterator it;

    while (pos.idx < ftext.length()) {
        const Pos start(pos);
        if (matches(RE_COMMENT) || matches(RE_SPACE))
            pos += res;
        else
            match_add_token(RE_INT, Token::Type::Int, std::stoll(res))
        else match_add_token(RE_FLOAT, Token::Type::Float, std::stold(res))
        else match_add_token(RE_STRING, Token::Type::String, res)
        else if ((it = std::find_if(keywords.begin(), keywords.end(),
                                    match_functor(ftext.substr(pos.idx), res)))
                 != keywords.end()) {
            pos += res;
            std::cout << "Found keyword: '" << res << "'\n";
            tokens.push_back({Token::Type::Keyword, res, start, pos});
        }
        else match_add_token(RE_IDENT, Token::Type::Ident, res)
        else if (matches(RE_LIT))
#define lit_match(l, t) \
    if (res == l) type = t, found = true
        {
            Token::LitType type;
            bool found = false;
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
            if (found) {
                pos += res;
                std::cout << "Found literal: '" << res << "'\n";
                tokens.push_back({Token::Type::Literal, type, start, pos});
            }
#undef lit_match
        }

        if (pos.idx == start.idx) return {};
    }

    return tokens;
#undef matches
#undef match_add_token
}
