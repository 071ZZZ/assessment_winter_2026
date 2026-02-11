#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#include "rm_a0/a0_05_score_stats.hpp"
namespace rm_a0{
  struct student {
    std::string name;
    int score;
  };
    std::string SolveScoreStats(std::string s,bool& ok){
      ok = false;
      std::istringstream iss(s);
      int n;
      iss >> n;
      std::vector<student> scores;
      int total = 0;
      int max = -1;
      std::string max_name;
      for (int i=0;i<n;i++){
        std::string name;
        int score;
        if(!(iss >> name >> score)){
          return "";
        }
        scores.push_back({name,score});
        total += score;
        if(score>max){
          max=score;
          max_name=name;
        }
      }
      double average = static_cast<double>(total) / n;
      std::ostringstream oss;
      oss << "top=" << max_name << " " << max << "\n";
      oss << std::fixed << std::setprecision(2) << "avg:" << average;
      ok = true;
      return oss.str();
    } 
}

int main() {
  std::ostringstream oss;
  oss << std::cin.rdbuf();

  bool ok = false;
  std::string out = rm_a0::SolveScoreStats(oss.str(), ok);
  if (!ok) {
    return 0;
  }

  std::cout << out;

  return 0;
}
