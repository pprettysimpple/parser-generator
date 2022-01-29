//
// Created by pprettysimpple on 25.01.2022.
//

#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <memory>

struct null_rule_t {};

inline constexpr null_rule_t null_rule;

struct rule_t {
  enum {
    NAME    = 0,
    PARAMS  = 1,
    CODE    = 2,
  };
  // name, parameters, code
  using elem_t = std::tuple<std::string, std::string, std::string>;

  rule_t();
  rule_t(null_rule_t) noexcept;

  void add(std::string name, std::string params, std::string code);
  std::vector<elem_t> get() const;
private:
  struct impl;
  std::shared_ptr<impl> pimpl;
};

rule_t make_null_rule();