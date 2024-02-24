/*  @file @brief @author chenchao @brief */

#include <iostream>
#include <string>
#include <vector>
#include "absl/strings/str_join.h"

/* @brief @pram @return */
int main() {
  std::vector<std::string> v = {"foo","bar","baz"};
  std::string s = absl::StrJoin(v, "-");

  std::cout << "Joined string: " << s << "\n";
}
