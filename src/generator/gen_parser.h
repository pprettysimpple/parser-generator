//
// Created by pprettysimpple on 25.01.2022.
//

#pragma once

#include "../grammar/grammar.h"

#include "gen_lexer.h"

struct parser_generator_t {
  explicit parser_generator_t(grammar_t grammar);

  std::string gen_parser_source();

  std::string gen_parser_header();
private:
  grammar_t grammar;

  std::string gen_parser_body();
};

inline constexpr auto EPS_TOKEN = "\"EPS\"";