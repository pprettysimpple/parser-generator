
#include "pretty_lexer_calc.h"
#include <string>
#include <vector>

			#include <iostream>
#include <vector>
#include <stdexcept>
 // usr-code


struct format_parser_t {
  explicit format_parser_t(std::string input);
	std::tuple<int> parse_e();

	std::tuple<int> parse_e_(int arg);

	std::tuple<int> parse_t();

	std::tuple<int> parse_t_(int arg);

	std::tuple<int> parse_w();

	std::tuple<int> parse_w_();

	std::tuple<int> parse_fact();

	std::tuple<int> parse_fact_(int arg);

	std::tuple<int> parse_f();

	std::tuple<int> parse_fact_op(int arg);

	std::tuple<int> parse_comb_op(int n,int k);

	std::tuple<int> parse_plus_op(int a,int b);

	std::tuple<int> parse_minus_op(int a,int b);

	std::tuple<int> parse_mul_op(int a,int b);

	std::tuple<int> parse_div_op(int a,int b);


private:
  format_lexer_t lex;
};
