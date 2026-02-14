#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include "rm_a0/a0_06_bigint.hpp"
namespace rm_a0
{
  BigInt::BigInt() : digits_({0}) {}
  BigInt::BigInt(const std::string& s) {
    digits_.clear();
    for (auto it=s.rbegin(); it!=s.rend(); ++it) {
        char c=*it;
        if (c>='0' && c<='9') {
            digits_.push_back(c-'0');
        }
    }
    if (digits_.empty()) {
        digits_.push_back(0);
    }
  }
  BigInt operator+(const BigInt& a,const BigInt& b){
    BigInt res;
    res.digits_.clear();
    int carry = 0;
    size_t max_len = std::max(a.digits_.size(), b.digits_.size());
    for (size_t i = 0; i < max_len || carry > 0; ++i) {
        int digit_a = (i < a.digits_.size()) ? a.digits_[i] : 0;
        int digit_b = (i < b.digits_.size()) ? b.digits_[i] : 0;
        int sum = digit_a + digit_b + carry;
        carry = sum / 10;
        res.digits_.push_back(sum % 10);
    }
    return res;
      };
      std::ostream& operator<<(std::ostream& os,const BigInt& x){
          for (auto it = x.digits_.rbegin(); it != x.digits_.rend(); ++it) {
        os << *it;
    }
    return os;
        }
    std::string SolveBigIntAdd(const std::string& input,bool& ok){
    std::istringstream in(input);
    std::string a;
    std::string b;
    std::ostringstream out;
    if (!std::getline(in, a)) {
        ok = false;
        return {};
    }
    if (!std::getline(in, b)) {
        ok = false;
        return {};
    }
    BigInt num_a(a);
    BigInt num_b(b);
    BigInt sum = num_a + num_b;
    out << sum;
    ok = true;
    return out.str();
  }
}


int main() {
  std::ostringstream oss;
  oss << std::cin.rdbuf();

  bool ok = false;
  std::string out = rm_a0::SolveBigIntAdd(oss.str(), ok);
  if (!ok) {
    return 0;
  }

  std::cout << out;
  return 0;
}
