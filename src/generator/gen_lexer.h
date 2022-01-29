//
// Created by pprettysimpple on 26.01.2022.
//

#pragma once

#include "../grammar/grammar.h"

struct lexer_generator_t {
  explicit lexer_generator_t(grammar_t gram);

  std::string gen_lexer_header();
  std::string gen_lexer_source();
private:
  grammar_t grammar;
};