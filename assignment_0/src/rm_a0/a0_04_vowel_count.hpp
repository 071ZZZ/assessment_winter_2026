#ifndef a0_04_vowel_count_hpp
#define a0_04_vowel_count_hpp
#include<string>
#include<cctype>
namespace rm_a0{
    bool IsVowelChar(unsigned char c);
    std::size_t CountVowels(const std::string& line);
}
#endif 