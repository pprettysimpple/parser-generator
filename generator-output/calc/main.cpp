//
// Created by pprettysimpple on 27.01.2022.
//
#include <iostream>
#include "pretty_parser_calc.h"

int main() {
  format_parser_t formatParser("1 + (10000 / 2) * 2");
  auto [result] = formatParser.parse_e();
  std::cout << result;
}
