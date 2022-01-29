//
// Created by pprettysimpple on 26.01.2022.
//

#pragma once

#include <string>
#include <stdexcept>
#include <memory>

template<typename ... Args>
std::string format(const std::string& format, Args... args)
{
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
  if(size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  auto buf = std::make_unique<char[]>(size);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);
}

std::string replace_all(std::string str, const std::string& from, const std::string& to);

void ltrim(std::string &s);

std::string ltrim_copy(std::string s);