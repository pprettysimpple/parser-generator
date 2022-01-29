//
// Created by pprettysimpple on 27.01.2022.
//

#include <utility>
#include <unordered_set>

#include "gtest/gtest.h"

#include "pretty_parser_variables_c.h"

// for debug
std::unordered_set<std::string> banned_keywords{
        "declare",
        "maybe_brackets",
        "names_continue",
};

std::string print_tree_dfs(const node_t& node) {
  const auto& children = node.get_children();
  std::string result;
  if (children.empty()
      && !banned_keywords.contains(node.get_name())) {
    result += node.get_name();
  }
  for (const auto& child : children) {
    result += print_tree_dfs(child);
  }
  return result;
}

auto eval(std::string input) {
  format_parser_t parser(std::move(input));
  auto [res] = parser.parse_declare();
  return print_tree_dfs(res);
}

TEST(Valid, Generic) {
  EXPECT_EQ(eval("int a[123], *b, **c, d;"), "int a[123],*b,**c,d;");
}

TEST(Valid, Simple) {
  EXPECT_EQ(eval("int a;"), "int a;");
}

TEST(Valid, Simple2) {
  EXPECT_EQ(eval("int a, b;"), "int a,b;");
}
