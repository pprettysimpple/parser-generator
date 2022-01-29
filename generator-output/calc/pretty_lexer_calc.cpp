
#include "pretty_lexer_calc.h"

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
	patterns.emplace_back(LBR, "LBR", std::regex("^\\("));
	patterns.emplace_back(RBR, "RBR", std::regex("^\\)"));
	patterns.emplace_back(PLUS, "PLUS", std::regex("^\\+"));
	patterns.emplace_back(MINUS, "MINUS", std::regex("^-"));
	patterns.emplace_back(MUL, "MUL", std::regex("^\\*"));
	patterns.emplace_back(DIV, "DIV", std::regex("^/"));
	patterns.emplace_back(DIGIT, "DIGIT", std::regex("^[0-9]+"));
	patterns.emplace_back(RUN_FACT, "RUN_FACT", std::regex("^!"));
	patterns.emplace_back(DOT_COMMA, "DOT_COMMA", std::regex("^;"));
	patterns.emplace_back(EPS, "EPS", std::regex("^EPS"));

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
