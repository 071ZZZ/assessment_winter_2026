#include <cctype>
#include <iostream>
#include <string>

#include "rm_a0/a0_04_vowel_count.hpp"
namespace rm_a0{
  int CountVowels(std::string line){
    int count=0;
    for(char c:line){
      char lower_c=std::tolower(c);
      if(lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || lower_c == 'o' || lower_c == 'u'){
        ++count;
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
