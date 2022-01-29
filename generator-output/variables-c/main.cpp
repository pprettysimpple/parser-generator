//
// Created by pprettysimpple on 27.01.2022.
//
#include <iostream>
#include <unordered_set>

#include "pretty_parser_variables_c.h"

std::string print_tree_graphvis_inner(const node_t& node) {
  std::string result;

  const auto& children = node.get_children();
  auto node_hash = std::to_string(node.get_id());

  result += node_hash + "[label=\"" + node.get_name() + "\"]\n";

  for (const auto& child : children) {
    auto child_hash = std::to_string(child.get_id());

    result += child_hash + "[label=\"" + child.get_name() + "\"]\n";

    result += node_hash + " -> ";
    result += child_hash + "\n";
  }

  result += "\n";
  for (const auto& child : children) {
    result += print_tree_graphvis_inner(child);
  }

  return result;
}

std::string print_graphvis(const node_t& node) {
  return "digraph {\n" + print_tree_graphvis_inner(node) + "}";
}

int main() {
  format_parser_t formatParser("int a[123], **b, *c;");
  auto [result] = formatParser.parse_declare();
  std::cout << print_graphvis(result) << std::endl;
  // next just put it in online graphviz "https://dreampuf.github.io/GraphvizOnline"
}