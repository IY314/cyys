#include "tokenizer.hpp"

#include <algorithm>
#include <iostream>
#include <regex>

// These rules are ignored
#define RE_COMMENT R"(^#.*?\n|\r$)"
#define RE_SPACE R"(^[ \t]+)"

// Number rules
#define RE_INT R"(^\d+)"
#define RE_FLOAT R"(^\d+\.\d+)"

// ONLY SINGLE QUOTES
#define RE_STRING "^'.*?'"

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
    const std::vector<std::string> keywords{
        R"(^require\b)", R"(^_\b)",    R"(^struct\b)", R"(^is\b)",
        R"(^end\b)",     R"(^func\b)", R"(^self\b)",   R"(^to\b)",
        R"(^proc\b)",    R"(^code\b)", R"(^let\b)",    R"(^if\b)",
        R"(^do\b)"},
        literals{R"(^\()", R"(^\))", R"(^\[)", R"(^\])", R"(^\{)", R"(^\})",
                 "^,",     R"(^\.)", R"(^\|)", "^:",     "^==",    "^=",
                 "^!=",    "^<=",    "^>=",    "^<",     "^>",     R"(^\+)",
                 "^-",     R"(^\*)", "^/"};

    std::vector<Token> tokens;

    Pos pos{0, 1, 0, fname, ftext};

    std::string res;

    std::vector<std::string>::const_iterator it;

    while (pos.idx < ftext.length()) {
        const Pos start(pos);

        if (matches(RE_COMMENT) || matches(RE_SPACE))
            pos += res;
        else
            match_add_token(R"(^(\n|\r)+)", Token::Type::Newline, res)
        else match_add_token(RE_INT, Token::Type::Int, std::stoll(res))
        else match_add_token(RE_FLOAT, Token::Type::Float, std::stold(res))
        else match_add_token(RE_STRING, Token::Type::String, res)
        else if ((it = std::find_if(keywords.begin(), keywords.end(),
                                    match_functor(ftext.substr(pos.idx), res)))
                 != keywords.end()) {
            pos += res;
            std::cout << "Found Token::Type::Keyword: '" << res << "'\n";
            tokens.push_back({Token::Type::Keyword, res, start, pos});
        }
        else match_add_token(RE_IDENT, Token::Type::Ident, res)
        else if ((it = std::find_if(literals.begin(), literals.end(),
                                    match_functor(ftext.substr(pos.idx), res)))
                 != literals.end()) {
            pos += res;
            std::cout << "Found Token::Type::Literal: '" << res << "'\n";
            tokens.push_back({Token::Type::Literal, res, start, pos});
        }

        if (pos.idx == start.idx) return {};
    }

    return tokens;
#undef matches
#undef match_add_token
}
