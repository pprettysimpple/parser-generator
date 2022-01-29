#include <iostream>

#include "src/generator/gen_parser.h"

#include "src/grammar/grammar.h"

#include "FormatLexer.h"
#include "FormatParser.h"

auto prepare_grammar(const std::string& code) {
  std::stringstream ss(code);
  antlr4::ANTLRInputStream
          input(ss);
  antlrcpptest::FormatLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  tokens.fill();
  antlrcpptest::FormatParser parser(&tokens);
  grammar_t grammar;
  parser.gram(grammar);
  return grammar;
}

std::string read_all_input(std::istream& in) {
  std::string acc, cur;
  while (std::getline(in, cur)) {
    acc += cur;
  }
  return acc;
}

void write_file(const std::string& filename, const std::string& content) {
  std::fstream file(filename, std::fstream::out);
  file << content;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: ./gen grammar_path output_dir_path" << std::endl;
    return 1;
  }
  std::string grammar_path = argv[1];
  std::string out_path = argv[2];

  std::fstream grammar_file(grammar_path, std::fstream::in);
  auto input = read_all_input(grammar_file);

  // grammar prepare
  grammar_t grammar = prepare_grammar(input);
  grammar.find_all();

  //  lexer generate
  lexer_generator_t lexer_generator(grammar);
  write_file(
          out_path + "pretty_lexer_" + grammar.get_name() + ".h",
          lexer_generator.gen_lexer_header()
  );
  write_file(
          out_path + "pretty_lexer_" + grammar.get_name() + ".cpp",
          lexer_generator.gen_lexer_source()
  );

  // parser generate
  parser_generator_t parser_generator(grammar);
  write_file(
          out_path + "pretty_parser_" + grammar.get_name() + ".h",
          parser_generator.gen_parser_header()
  );
  write_file(
          out_path + "pretty_parser_" + grammar.get_name() + ".cpp",
          parser_generator.gen_parser_source()
  );
  return 0;
}
