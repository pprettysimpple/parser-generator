
#include <utility>
#include <regex>
#include <vector>
#include <string>

enum token_t {
  END,
  LBR,
RBR,
PLUS,
MINUS,
MUL,
DIV,
DIGIT,
RUN_FACT,
DOT_COMMA,
EPS,

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
