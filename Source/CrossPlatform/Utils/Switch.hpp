#ifndef SWITCH_FILE_H
#define SWITCH_FILE_H

#include <iostream>
#include <string>
#include <vector>

struct Switch {

  using void_function = std::function<void(void)>;
  std::unordered_map<std::string, void_function> cases;

  void add_case(std::string const & key, void_function func) {
    cases[key] = func;
  }

  std::vector<std::string> all_keys() {
    std::vector<std::string> keys;
    for (const auto&elem : cases) {
      keys.emplace_back(elem.first);
    }
    return keys;
  }

  Switch() {}
  void operator()(std::string const & key) {
    auto f = cases.find(key);
    f = (f == cases.end()) ? cases.find("default") : f;
    if (f == cases.end()) {
      std::cout << "Case not found and default not set\n";
      return;
    }

    f->second();
  }
};

#endif
