//
// Created by pprettysimpple on 25.01.2022.
//

#pragma once

#include <memory>

#include "rule.h"
#include "term.h"

struct nterm_t {
  using param_t = std::tuple<std::string, std::string>;
  using return_t = param_t;

  nterm_t();
  nterm_t(const nterm_t& rhs) = default;
  nterm_t(nterm_t&& rhs) noexcept = default;

  nterm_t& operator=(const nterm_t& rhs) = default;
  nterm_t& operator=(nterm_t&& rhs) noexcept = default;

  std::string get_name() const;
  void set_name(std::string new_name);

  void add_rule(rule_t rule);
  void add_param(std::string type, std::string pname);
  void add_return(std::string type, std::string name);
  std::vector<rule_t> get_rules() const;
  std::vector<param_t> get_params() const;
  std::vector<return_t> get_returns() const;
  std::unordered_set<term_t>& get_first();
  std::unordered_set<term_t>& get_follow();
  const std::unordered_set<term_t>& get_first() const;
  const std::unordered_set<term_t>& get_follow() const;

private:
  struct impl;
  std::shared_ptr<impl> pimpl;
};