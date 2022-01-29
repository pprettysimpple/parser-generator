//
// Created by pprettysimpple on 25.01.2022.
//

#include "term.h"

term_t::term_t(std::string name, std::string rgx)
        : name(std::move(name)), rgx(std::move(rgx)) {}

bool operator==(const term_t &lhs, const term_t &rhs) {
  return lhs.rgx == rhs.rgx;
}

bool operator!=(const term_t &lhs, const term_t &rhs) {
  return !(lhs == rhs);
}

