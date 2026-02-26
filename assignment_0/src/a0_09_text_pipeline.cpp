#include "rm_a0/a0_lib.hpp"

#include <iostream>
#include <string>
#include <vector>
namespace rm_a0{
  std::vector<std::string> RunTextPipeline(const std::string& pipeline, const std::vector<std::string>& lines, bool& ok) {
    (void)pipeline;
    (void)lines;
    ok = false;
    return {};
}
}
int main() {
  std::string pipeline;
  if (!std::getline(std::cin, pipeline)) return 0;

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) lines.push_back(line);

  bool ok = false;
  auto out = rm_a0::RunTextPipeline(pipeline, lines, ok);
  if (!ok) {
    std::cout << "FAIL\n";
    return 0;
  }

  for (const auto& s : out) std::cout << s << "\n";
  return 0;
}
