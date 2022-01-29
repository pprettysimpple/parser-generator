//
// Created by pprettysimpple on 26.01.2022.
//

#include "util.h"

std::string replace_all(std::string str, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
  }
  return str;
}

void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
}

std::string ltrim_copy(std::string s) {
  ltrim(s);
  return s;
}