//
// Created by pprettysimpple on 25.01.2022.
//

#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include <variant>
#include "rule.h"
#include "term.h"
#include "nterm.h"

struct grammar_t {
  grammar_t();
  void set_name(std::string new_name);
  std::string get_name() const noexcept;

  void set_start(std::string new_start);
  std::string get_start() const noexcept;

  void add_term(const std::string& name, std::string rule);
  std::vector<term_t> get_terms() const noexcept;

  void add_nterm(const nterm_t& nterm);
  std::vector<nterm_t> get_nterms() const noexcept;

  std::string get_first_repr();
  std::string get_follow_repr();

  void find_all();

  std::unordered_set<term_t> find_rule_first(const rule_t& rule, size_t idx);

  std::unordered_set<term_t> first1(nterm_t nterm, const rule_t& rule, size_t idx);

  std::variant<nterm_t, term_t> get_by_name(const std::string& name);

  void add_header_code(const std::string& code);

  std::string get_header_code();

private:
  struct impl;
  std::shared_ptr<impl> pimpl;
};
