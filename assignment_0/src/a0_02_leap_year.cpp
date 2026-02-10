#include <iostream>
#include <iomanip>
#include <sstream>

#include "rm_a0/a0_02_leap_year.hpp"
namespace rm_a0{
  bool IsLeapYear(int year){
    return (year%4==0 && year%100!=0) || (year%400==0);
  }
  std::string FormatLeapYearAnswer(bool is_leap){
    if(is_leap){
      return "YES";
    }
    else
      return "NO";
  }
}
int main() {
  int year = 0;
  if (!(std::cin >> year)) {
    return 0;
  }

  std::cout << rm_a0::FormatLeapYearAnswer(rm_a0::IsLeapYear(year));

  return 0;
}
