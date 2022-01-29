//
// Created by pprettysimpple on 27.01.2022.
//

#include <utility>

#include "gtest/gtest.h"

#include "pretty_parser_calc.h"

auto eval(std::string input) {
  format_parser_t parser(std::move(input));
  auto [res] = parser.parse_e();
  return res;
}

TEST(Valid, Simple) {
  EXPECT_EQ(eval("12"), 12);
  EXPECT_EQ(eval("2 + 2"), 4);
  EXPECT_EQ(eval("2 + 2 * 2"), 6);
  EXPECT_EQ(eval("(2 + 2) * 2"), 8);
  EXPECT_EQ(eval("2 - (2 + 2) * 2"), -6);
}

TEST(Valid, DivAssoc) {
  EXPECT_EQ(eval("4 / 2 / 2"), 1);
}

TEST(Valid, SubAssoc) {
  EXPECT_EQ(eval("4 - 2 - 2"), 0);
}

TEST(Valid, FactorialPriority) {
  EXPECT_EQ(eval("10 - 2!"), 8);
}

TEST(Valid, SomeBrackets) {
  EXPECT_EQ(eval("((10 + (0)))"), 10);
}

TEST(Valid, Combinations) {
  EXPECT_EQ(eval("5 ; 2"), 10);
}

TEST(Valid, CombinationsAssoc) {
  EXPECT_EQ(eval("12 ; 5 ; 2"), 66);
}


TEST(Invalid, BracketsSeq) {
  try {
    eval("((22 + (0))"); // wrong brackets seq
  } catch (const std::logic_error& err) {
    EXPECT_EQ(err.what(), std::string("Unexpected token: )\nWhen processing nterm: f"));
  } catch (...) {
    FAIL() << "Expected std::logic_error";
  }
}

TEST(Invalid, DivByZero) {
  try {
    eval("22 / 0"); // divide by zero
  } catch (const std::logic_error& err) {
    EXPECT_EQ(err.what(), std::string("Divide by zero"));
  } catch (...) {
    FAIL() << "Expected std::logic_error";
  }
}