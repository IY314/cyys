#pragma once

#include <vector>

#include "token.hpp"

std::vector<Token> tokenize(const std::string& fname,
                            const std::string& ftext) noexcept;
