#ifndef a0_06_bigint_hpp
#define a0_06_bigint_hpp
#include <string>
#include <ostream>
#include <vector>
namespace rm_a0{
    class BigInt {
    public:
    BigInt();
    explicit BigInt(const std::string &s);
    friend BigInt operator+(const BigInt &a, const BigInt &b);
    friend std::ostream &operator<<(std::ostream &os, const BigInt &x);
    friend std::string SolveBigIntAdd(const std::string& input,bool& ok);
private:
    std::vector<int> digits_;
};
}
#endif  