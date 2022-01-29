//
// Created by pprettysimpple on 25.01.2022.
//

#include <unordered_map>
#include <utility>
#include "grammar.h"
#include "../generator/gen_parser.h"

static const term_t END = term_t("END", "");

struct grammar_t::impl {
  void set_name(std::string new_name);
  std::string get_name() const noexcept;

  void set_start(std::string new_start);
  std::string get_start() const noexcept;

  void add_term(const std::string& term_name, std::string rule);
  std::vector<term_t> get_terms() const;

  void add_nterm(const nterm_t& nterm);
  std::vector<nterm_t> get_nterms() const;

  std::string get_first_repr();
  std::string get_follow_repr();

  void find_all();

  void find_eps();

  void find_first();
  void find_follow();

  std::unordered_set<term_t> find_rule_first(const rule_t& rule, size_t idx);

  std::unordered_set<term_t> first1(nterm_t nterm, const rule_t& rule, size_t idx);

  std::variant<nterm_t, term_t> get_by_name(const std::string &name);

  void add_header_code(const std::string& code);

  std::string get_header_code() const noexcept;

private:
  std::string name;
  std::string start_name;
  std::string header_code;
  std::unordered_map<std::string, term_t> term_from_name;
  std::vector<term_t> terms;
  std::unordered_map<std::string, nterm_t> nterm_from_name;
  std::vector<nterm_t> nterms;
  term_t EPS_TERM;

  bool check_LL1();
};

// interface

void grammar_t::set_name(std::string new_name) {
  return pimpl->set_name(std::move(new_name));
}

std::string grammar_t::get_name() const noexcept {
  return pimpl->get_name();
}

void grammar_t::set_start(std::string new_start) {
  return pimpl->set_start(std::move(new_start));
}

std::string grammar_t::get_start() const noexcept {
  return pimpl->get_start();
}

void grammar_t::add_term(const std::string& name, std::string rule) {
  return pimpl->add_term(name, std::move(rule));
}

std::vector<term_t> grammar_t::get_terms() const noexcept {
  return pimpl->get_terms();
}

void grammar_t::add_nterm(const nterm_t &nterm) {
  return pimpl->add_nterm(nterm);
}

std::vector<nterm_t> grammar_t::get_nterms() const noexcept {
  return pimpl->get_nterms();
}

std::string grammar_t::get_first_repr() {
  return pimpl->get_first_repr();
}

std::string grammar_t::get_follow_repr() {
  return pimpl->get_follow_repr();
}

void grammar_t::find_all() {
  return pimpl->find_all();
}

std::unordered_set<term_t> grammar_t::find_rule_first(const rule_t& rule, size_t idx) {
  return pimpl->find_rule_first(rule, idx);
}

std::unordered_set<term_t> grammar_t::first1(nterm_t nterm, const rule_t& rule, size_t idx) {
  return pimpl->first1(std::move(nterm), rule, idx);
}

grammar_t::grammar_t() : pimpl(std::make_shared<grammar_t::impl>()) {}

std::variant<nterm_t, term_t> grammar_t::get_by_name(const std::string &name) {
  return pimpl->get_by_name(name);
}

void grammar_t::add_header_code(const std::string &code) {
  return pimpl->add_header_code(code);
}

std::string grammar_t::get_header_code() {
  return pimpl->get_header_code();
}

// impl

void grammar_t::impl::set_name(std::string new_name) {
  name = std::move(new_name);
}

std::string grammar_t::impl::get_name() const noexcept {
  return name;
}

void grammar_t::impl::set_start(std::string new_start) {
  start_name = std::move(new_start);
}

std::string grammar_t::impl::get_start() const noexcept {
  return start_name;
}

void grammar_t::impl::add_term(const std::string& term_name, std::string rule) {
  term_t term = term_t(term_name, std::move(rule));
  terms.push_back(term);
  term_from_name[term_name] = term;
}

std::vector<term_t> grammar_t::impl::get_terms() const {
  return terms;
}

void grammar_t::impl::add_nterm(const nterm_t& nterm) {
  nterms.push_back(nterm);
  nterm_from_name[nterm.get_name()] = nterm;
}

std::vector<nterm_t> grammar_t::impl::get_nterms() const {
  return nterms;
}

std::string grammar_t::impl::get_first_repr() {
  std::string result = "first:\n\n";
  for (const auto& nterm : nterms) {
    result += nterm.get_name() + "\n";
    for (const auto& term : nterm.get_first()) {
      result += "\t" + term.name + "\n";
    }
  }
  return result;
}

std::string grammar_t::impl::get_follow_repr() {
  std::string result = "follow:\n\n";
  for (const auto& nterm : nterms) {
    result += nterm.get_name() + "\n";
    for (const auto& term : nterm.get_follow()) {
      result += "\t" + term.name + "\n";
    }
  }
  return result;
}

void grammar_t::impl::find_all() {
  this->find_eps();
  this->find_first();
  this->find_follow();
  if (!this->check_LL1()) {
    throw std::logic_error("Not LL1 grammar.");
  }
}

namespace {
bool is_eps(const term_t& term) {
  return term.rgx == EPS_TOKEN;
}
}

void grammar_t::impl::find_eps() {
  for (const term_t& term : terms) {
    if (is_eps(term)) {
      EPS_TERM = term;
      break;
    }
  }
}

void grammar_t::impl::find_first() {
  bool retry = true;
  while (retry) {
    retry = false;
    for (auto& nterm : nterms) {
      for (const auto& rule : nterm.get_rules()) {
        auto size = nterm.get_first().size();
        for (const auto& term : find_rule_first(rule, 0)) {
          nterm.get_first().insert(term);
        }
        if (size != nterm.get_first().size()) {
          retry = true;
        }
      }
    }
  }
}

void grammar_t::impl::find_follow() {
  auto s_nterm = nterm_from_name[start_name];
  s_nterm.get_follow().insert(END);
  bool retry = true;
  while (retry) {
    retry = false;
    for (const auto& nterm : nterms) {
      for (const auto& rule : nterm.get_rules()) {
        auto elems = rule.get();
        for (size_t i = 0; i < elems.size(); i++) {
          const auto& e = elems[i];
          auto it = nterm_from_name.find(std::get<rule_t::NAME>(e));
          if (it == nterm_from_name.end()) {
            continue;
          }
          auto next_nterm = it->second;
          auto copy_size = next_nterm.get_follow().size();
          if (i + 1 != elems.size()) {
            auto rest_first = find_rule_first(rule, i + 1);
            if (rest_first.contains(EPS_TERM)) {
              rest_first.erase(EPS_TERM);
              for (const auto& cur : nterm.get_follow()) {
                rest_first.insert(cur);
              }
            }
            for (const auto& cur : rest_first) {
              next_nterm.get_follow().insert(cur);
            }
          } else {
            for (const auto& cur : nterm.get_follow()) {
              next_nterm.get_follow().insert(cur);
            }
          }
          if (copy_size != next_nterm.get_follow().size()) {
            retry = true;
          }
        }
      }
    }
  }
}

std::unordered_set<term_t> grammar_t::impl::find_rule_first(const rule_t& rule, size_t idx) {
  std::unordered_set<term_t> result;
  const auto& elems = rule.get();
  if (elems.size() <= idx) {
    return result;
  }
  auto e = elems[idx];
  auto it = term_from_name.find(std::get<rule_t::NAME>(e));
  if (it != term_from_name.end()) {
    const auto& term = it->second;
    result.insert(term);
    return result;
  }
  const auto& nterm = nterm_from_name[std::get<rule_t::NAME>(e)];
  for (const auto& cur : nterm.get_first()) {
    result.insert(cur);
  }
  if (!result.contains(EPS_TERM)) {
    return result;
  }
  result.erase(EPS_TERM);
  if (idx + 1 < elems.size()) {
    for (const auto& cur : find_rule_first(rule, idx + 1)) {
      result.insert(cur);
    }
  } else {
    result.insert(EPS_TERM);
  }
  return result;
}

// first(\alpha) unite (follow(A) if eps \in \alpha)
std::unordered_set<term_t> grammar_t::impl::first1(nterm_t nterm, const rule_t& rule, size_t idx) {
  std::unordered_set<term_t> res = find_rule_first(rule, idx);

  if (!res.contains(EPS_TERM)) {
    return res;
  }

  res.erase(EPS_TERM);
  for (const auto& cur : nterm.get_follow()) {
    res.insert(cur);
  }

  return res;
}

std::variant<nterm_t, term_t> grammar_t::impl::get_by_name(const std::string &str) {
  auto it_term = term_from_name.find(str);
  if (it_term != term_from_name.end()) {
    return it_term->second;
  }
  auto it_nterm = nterm_from_name.find(str);
  if (it_nterm != nterm_from_name.end()) {
    return it_nterm->second;
  }
  throw std::logic_error("Unknown name for term/nterm.");
}

void grammar_t::impl::add_header_code(const std::string& code) {
  header_code += code;
}

std::string grammar_t::impl::get_header_code() const noexcept {
  return header_code;
}

bool grammar_t::impl::check_LL1() {
  for (const auto& nterm : nterms) {
    for (const auto& ruleA : nterm.get_rules()) {
      for (const auto& ruleB : nterm.get_rules()) {
        if (ruleA.get() == ruleB.get()) { // no equals for rules, but for vectors and elems
          continue;
        }

        auto first1A = this->first1(nterm, ruleA, 0);
        auto firstB = this->find_rule_first(ruleB, 0);

        // check intersection
        for (const auto& a : first1A) {
          for (const auto& b : firstB) {
            if (a == b) {
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}
