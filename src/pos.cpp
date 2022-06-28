#include "pos.hpp"

void Pos::operator+=(const std::string& passed) noexcept {
    for (const char& c : passed) {
        idx++, col++;

        if (c == '\n') {
            ln++;
            col = 0;
        }
    }
};
