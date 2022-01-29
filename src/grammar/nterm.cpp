//
// Created by pprettysimpple on 25.01.2022.
//

#include <unordered_set>
#include <utility>
#include "nterm.h"

struct nterm_t::impl {
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
  std::string name;
  std::vector<rule_t> rules;
  std::vector<param_t> params;
  std::vector<return_t> returns;
  std::unordered_set<term_t> first;
  std::unordered_set<term_t> follow;
};

// interface

std::string nterm_t::get_name() const {
  return pimpl->get_name();
}

void nterm_t::set_name(std::string new_name) {
  return pimpl->set_name(std::move(new_name));
}

void nterm_t::add_rule(rule_t rule) {
  return pimpl->add_rule(std::move(rule));
}

void nterm_t::add_param(std::string type, std::string pname) {
  return pimpl->add_param(std::move(type), std::move(pname));
}

void nterm_t::add_return(std::string type, std::string name) {
  return pimpl->add_return(std::move(type), std::move(name));
}

std::vector<rule_t> nterm_t::get_rules() const {
  return pimpl->get_rules();
}

std::vector<nterm_t::param_t> nterm_t::get_params() const {
  return pimpl->get_params();
}

std::vector<nterm_t::return_t> nterm_t::get_returns() const {
  return pimpl->get_returns();
}

std::unordered_set<term_t>& nterm_t::get_first() {
  return pimpl->get_first();
}

std::unordered_set<term_t>& nterm_t::get_follow() {
  return pimpl->get_follow();
}

const std::unordered_set<term_t>& nterm_t::get_first() const {
  return pimpl->get_first();
}

const std::unordered_set<term_t>& nterm_t::get_follow() const {
  return pimpl->get_follow();
}

nterm_t::nterm_t() :pimpl(std::make_shared<nterm_t::impl>()) {}

// impl

std::string nterm_t::impl::get_name() const {
  return name;
}

void nterm_t::impl::set_name(std::string new_name) {
  this->name = std::move(new_name);
}

void nterm_t::impl::add_rule(rule_t rule) {
  rules.push_back(std::move(rule));
}

void nterm_t::impl::add_param(std::string type, std::string pname) {
  params.emplace_back(std::move(type), std::move(pname));
}

void nterm_t::impl::add_return(std::string type, std::string rname) {
  returns.emplace_back(std::move(type), std::move(rname));
}

std::vector<rule_t> nterm_t::impl::get_rules() const {
  return rules;
}

std::vector<nterm_t::param_t> nterm_t::impl::get_params() const {
  return params;
}

std::vector<nterm_t::return_t> nterm_t::impl::get_returns() const {
  return returns;
}

std::unordered_set<term_t> &nterm_t::impl::get_first() {
  return first;
}

std::unordered_set<term_t> &nterm_t::impl::get_follow() {
  return follow;
}

const std::unordered_set<term_t> &nterm_t::impl::get_first() const {
  return first;
}

const std::unordered_set<term_t> &nterm_t::impl::get_follow() const {
  return follow;
}
