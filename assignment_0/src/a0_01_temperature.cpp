#include <iostream>
#include<iomanip>
#include<sstream>
#include "rm_a0/a0_01_temperature.hpp"
namespace rm_a0{
  double CelsiusToFahrenheit(double celsius) {
    return celsius * 9.0 / 5.0 + 32.0;
}

std::string FormatFahrenheit(double fahrenheit) {//格式化输出为保留2位小数的字符串
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << fahrenheit;
    return oss.str();
}
}
int main() {
  double celsius = 0.0;
  if (!(std::cin >> celsius)) {
    return 0;
  }
  std::cout << rm_a0::FormatFahrenheit(rm_a0::CelsiusToFahrenheit(celsius));
  return 0;
}
