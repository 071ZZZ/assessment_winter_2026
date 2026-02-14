#include <cctype>
#include <iostream>
#include <string>

#include "rm_a0/a0_04_vowel_count.hpp"
namespace rm_a0{
  bool IsVowelChar(unsigned char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
  }
  std::size_t CountVowels(const std::string& line) {
    std::size_t count = 0;
    for (char ch : line) {
      if (rm_a0::IsVowelChar(static_cast<unsigned char>(ch))) {
        count++;
      }
    }
    return count;
}
}
int main() {
  std::string line;
  if (!std::getline(std::cin, line)) {
    return 0;
  }

  std::cout << rm_a0::CountVowels(line) << "\n";

  return 0;
}
