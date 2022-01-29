//
// Created by pprettysimpple on 25.01.2022.
//

#include "rule.h"

#include <utility>

struct rule_t::impl {
  void add(std::string name, std::string params, std::string code);
  std::vector<elem_t> get() const;

private:
  std::vector<elem_t> elems;
};

void rule_t::add(std::string name, std::string params, std::string code) {
  return pimpl->add(std::move(name), std::move(params), std::move(code));
}

std::vector<rule_t::elem_t> rule_t::get() const {
  return pimpl->get();
}

rule_t::rule_t() : pimpl(std::make_shared<rule_t::impl>()) {}

rule_t::rule_t(null_rule_t) noexcept
: pimpl(nullptr) {}

// impl

void rule_t::impl::add(std::string name, std::string params, std::string code) {
  elems.emplace_back(std::move(name), std::move(params), std::move(code));
}

std::vector<rule_t::elem_t> rule_t::impl::get() const {
  return elems;
}
