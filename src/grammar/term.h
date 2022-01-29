//
// Created by pprettysimpple on 25.01.2022.
//

#pragma once

#include <string>

struct term_t {
  term_t(std::string name, std::string rgx);
  term_t() noexcept = default;

  term_t(const term_t& rhs) = default;
  term_t(term_t&& rhs) = default;

  term_t& operator=(const term_t& rhs) = default;
  term_t& operator=(term_t&& rhs) = default;

  friend bool operator==(const term_t& lhs, const term_t& rhs);
  friend bool operator!=(const term_t& lhs, const term_t& rhs);

  std::string name; // public
  std::string rgx;
};

template<>
struct std::hash<term_t>
{
  std::size_t operator()(const term_t& term) const {
    return std::hash<std::string>()(term.rgx);
  }
};