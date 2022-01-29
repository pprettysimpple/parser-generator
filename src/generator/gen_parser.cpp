//
// Created by pprettysimpple on 25.01.2022.
//

#include "gen_parser.h"
#include "util.h"

#include <utility>

namespace {
std::string get_nterm_result_type(const nterm_t& nterm) {
  if (nterm.get_returns().empty()) {
    return "void";
  }
  std::string result = "std::tuple<";
  for (const auto& [type, name] : nterm.get_returns()) {
    result += type + ",";
  }
  result.pop_back(); // remove last comma
  result += ">";
  return result;
}

std::string get_nterm_result_names(const nterm_t& nterm) {
  if (nterm.get_returns().empty()) {
    return "";
  }
  std::string result;
  for (const auto& [type, name] : nterm.get_returns()) {
    result += "res_" + name + ",";
  }
  result.pop_back(); // remove last comma
  return result;
}

std::string get_nterm_result_names_initialize(const nterm_t& nterm, const std::string& delim) {
  if (nterm.get_returns().empty()) {
    return "";
  }
  std::string result;
  for (const auto& [type, name] : nterm.get_returns()) {
    result += delim;
    result += type + " ";
    result += "res_" + name;
    result += ";\n";
  }
  return result;
}

std::string get_nterm_result_names_indexed(const nterm_t& nterm, size_t idx) {
  if (nterm.get_returns().empty()) {
    return "";
  }
  std::string result;
  for (const auto& [type, name] : nterm.get_returns()) {
    result += nterm.get_name() + std::to_string(idx) + "_" + name + ",";
  }
  result.pop_back(); // remove last comma
  return result;
}

std::string get_nterm_func_name(const nterm_t& nterm) {
  return "parse_" + nterm.get_name();
}

std::string get_nterm_args_list(const nterm_t& nterm) {
  std::string result;
  for (const auto& [type, name] : nterm.get_params()) {
    result += type + " ";
    result += name + ",";
  }
  if (!nterm.get_params().empty()) {
    result.pop_back(); // remove comma
  }
  return result;
}

std::string prepare_user_code(std::string user_code) {
  if (user_code.empty()) {
    return "";
  }
  user_code = ltrim_copy(std::move(user_code));
  user_code = replace_all(user_code, "<br>", "\n");
  return "\t\t\t" +
          user_code +
          " // usr-code\n"; // user-code
}

std::string get_nterm_func_decl(const grammar_t& grammar, const nterm_t& nterm) {
  std::string result = get_nterm_result_type(nterm) + " ";
  result += get_nterm_func_name(nterm);
  result += "(" + get_nterm_args_list(nterm) + ")";
  return result;
}

std::string get_nterm_func_decl_qualified(const grammar_t& grammar, const nterm_t& nterm) {
  std::string result = get_nterm_result_type(nterm) + " ";
  result += "format_parser_t::" + get_nterm_func_name(nterm);
  result += "(" + get_nterm_args_list(nterm) + ")";
  return result;
}

std::string get_nterm_func(grammar_t grammar, const nterm_t& nterm) {
  std::string result = get_nterm_func_decl_qualified(grammar, nterm) + " {\n";
  result += get_nterm_result_names_initialize(nterm, "\t");
  result += "\tswitch (lex.get_token()) {\n";
  for (const auto& rule : nterm.get_rules()) {
    for (const auto& term : grammar.first1(nterm, rule, 0)) {
      result += format("\t\tcase (%s):\n", term.name.c_str());
    }
    result += "\t\t{\n";
    const auto& rule_elements = rule.get();
    for (size_t idx = 0; idx < rule_elements.size(); idx++) {
      const auto& [name, params, code] = rule_elements[idx];

      auto name_copy = name; // can not capture bindings in lambda
      auto params_copy = params; // can not capture bindings in lambda
      auto code_copy = code; // can not capture bindings in lambda

      auto tmp = grammar.get_by_name(name);
      std::visit([&] (const auto& cur) { // why not? just for fun :)
        if constexpr (std::is_same_v<std::remove_cvref_t<decltype(cur)>, term_t>) {
          // term case
          auto term = static_cast<term_t>(cur);
          if (term.rgx == EPS_TOKEN) {
            result += prepare_user_code(code_copy);
          } else {
            result += "\t\t\tif (lex.get_token() != " + name_copy + ")\n";
            result += "\t\t\t\tthrow std::logic_error("; // rise error
            result += R"("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: )"; // rise error
            result += nterm.get_name() + "\");\n"; // rise error
            result += prepare_user_code(code_copy); // first we call user code, next get next token
            result += "\t\t\tlex.next_token();\n";
          }
        } else if constexpr (std::is_same_v<std::remove_cvref_t<decltype(cur)>, nterm_t>) {
          // nterm case
          auto rule_nterm = static_cast<nterm_t>(cur);
          auto names_str = get_nterm_result_names_indexed(rule_nterm, idx);
          result += "\t\t\t";
          if (!names_str.empty()) {
            result += "auto [" + names_str + "] = "; // if there is names for
          }
          result += get_nterm_func_name(rule_nterm); // call function to parse nterm
          result += "(" + params_copy + ");\n"; // call args
          result += prepare_user_code(code_copy);
        } else {
          throw std::logic_error("Unknown alternative.");
        }
      }, tmp);
    }
    result += "\t\t\treturn {" + get_nterm_result_names(nterm) + "};\n";
    result += "\t\t}\n";
  }
  result += "\t\tdefault:\n"; // rise error
  result += "\t\t\tthrow std::logic_error("; // rise error
  result += R"("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: )"; // rise error
  result += nterm.get_name() + "\");\n"; // rise error
  result += "\t}\n";
  result += "}";
  return result;
}

std::string get_parser_functions_source(const grammar_t& grammar) {
  std::string result;
  for (const auto& nterm : grammar.get_nterms()) {
    result += get_nterm_func(grammar, nterm) + "\n\n";
  }
  return result;
}

} // namespace

parser_generator_t::parser_generator_t(grammar_t grammar)
: grammar(std::move(grammar)) {}

std::string parser_generator_t::gen_parser_header() {
  std::string result = format(R"(
#include "pretty_lexer_%s.h"
#include <string>
#include <vector>

%s

struct format_parser_t {
  explicit format_parser_t(std::string input);
%s
private:
  format_lexer_t lex;
};
)"
, grammar.get_name().c_str()
, prepare_user_code(grammar.get_header_code()).c_str()
, parser_generator_t::gen_parser_body().c_str());
  return result;
}

std::string parser_generator_t::gen_parser_body() {
  std::string result;
  for (const auto& nterm : grammar.get_nterms()) {
    result += "\t" + get_nterm_func_decl(grammar, nterm) + ";\n\n";
  }
  return result;
}

std::string parser_generator_t::gen_parser_source() {
  std::string result = format(R"(
#include "pretty_parser_%s.h"

format_parser_t::format_parser_t(std::string input)
: lex(std::move(input)) {}

%s
)"
, grammar.get_name().c_str()
, get_parser_functions_source(grammar).c_str());
  return result;
}