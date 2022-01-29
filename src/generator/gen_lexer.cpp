//
// Created by pprettysimpple on 26.01.2022.
//

#include "gen_lexer.h"
#include "util.h"

#include <utility>
#include <regex>

lexer_generator_t::lexer_generator_t(grammar_t gram)
: grammar(std::move(gram)) {}

std::string lexer_generator_t::gen_lexer_header() {
  std::string format_lexer = R"(
#include <utility>
#include <regex>
#include <vector>
#include <string>

enum token_t {
  END,
  %s
};

struct format_lexer_t {
  explicit format_lexer_t(std::string input);

  token_t get_token() const noexcept;
  std::string get_value() const noexcept;

  token_t next_token();
private:
  const std::string input;
  std::vector<std::tuple<token_t, std::string, std::regex>> patterns;
  std::string cur_value;
  token_t cur_token;
  size_t cur_pos = 0;
};
)";
  std::string enum_inner;
  for (const auto& term : grammar.get_terms()) {
    enum_inner += term.name + ",\n";
  }
  return format(format_lexer, enum_inner.c_str());
}

std::string lexer_generator_t::gen_lexer_source() {
  std::string format_lexer = R"(
#include "pretty_lexer_%s.h"

namespace {
bool is_whitespace(char c) {
  if (c == ' ' || c == '\n'
      || c == '\r' || c == '\t') {
    return true;
  }
  return false;
}

size_t skip_space(const std::string& in, size_t pos) {
  while (pos < in.size() && is_whitespace(in[pos])) {
    pos++;
  }
  return pos;
}
} // namespace

format_lexer_t::format_lexer_t(std::string input)
        : input(std::move(input)) {
%s
  next_token();
}

token_t format_lexer_t::get_token() const noexcept {
  return cur_token;
}

std::string format_lexer_t::get_value() const noexcept {
  return cur_value;
}

token_t format_lexer_t::next_token() {
  cur_pos = skip_space(input, cur_pos);

  if (cur_pos == input.size()) {
    return cur_token = token_t::END;
  }

  for (const auto& [tok, name, regex] : patterns) {
    std::smatch results;
    if (std::regex_search(input.begin() + cur_pos, input.end(), results, regex)) {
      cur_value = results[results.size() - 1];
      cur_token = tok;
      cur_pos += cur_value.size();
      return cur_token;
    }
  }
  throw std::logic_error("Unexpected input: " + input.substr(cur_pos, cur_pos + 10));
}
)";
  std::string add_patterns;
  for (const auto& term : grammar.get_terms()) {
    auto rgx_copy = term.rgx;
    rgx_copy.insert(1, "^"); // insert carry to match prefix of string
    add_patterns += format("\tpatterns.emplace_back(%s, %s, std::regex(%s));",
                           term.name.c_str(),
                           ("\"" + term.name + "\"").c_str(),
                           rgx_copy.c_str()) + "\n";
  }
  return format(format_lexer, grammar.get_name().c_str(), add_patterns.c_str());
}
