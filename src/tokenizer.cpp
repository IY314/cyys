#include "tokenizer.hpp"

#include <iostream>
#include <regex>

#define RE_SPACE  R"(^\s+)"
#define RE_INT    R"(^\d+)"
#define RE_FLOAT  R"(^\d+\.\d+)"
#define RE_STRING R"(^'.*?')"
#define RE_KEYWORD  // TODO
#define RE_IDENT R"(^[[:alpha:]]\w*)"

bool match(const std::string &str, const std::string &regex, std::string &res)
{
    std::cout << "Matching " << str << " against " << regex << '\n';
    std::smatch m;
    bool r = std::regex_search(str, m, std::regex(regex));
    if (r) res = m[0];
    return r;
}

#define match_add_token(rgx, typ, cnv)             \
    if (match(ftext.substr(start.idx), rgx, res))  \
    {                                              \
        std::cout << "found " << res << std::endl; \
        pos += res;                                \
        tokens.push_back({typ, cnv, start, pos});  \
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
        if (match(ftext.substr(start.idx), RE_SPACE, res))
        {
            std::cout << "found space(s)\n";
            pos += res;
        }
        else
            match_add_token(RE_INT, Token::Type::Int, std::stoll(res))
        else match_add_token(RE_FLOAT, Token::Type::Float, std::stold(res))
        else match_add_token(RE_STRING, Token::Type::String, res)
        // TODO: keyword
        else match_add_token(RE_IDENT, Token::Type::Ident, res)

        if (pos.idx == start.idx) { return {}; }
    }

    return tokens;
}
