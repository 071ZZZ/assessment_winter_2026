#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#include "rm_a0/a0_05_score_stats.hpp"
namespace rm_a0{
  ScoreStatsResult ComputeScoreStats(const std::string& input, bool& ok) {
    ok = false;
    ScoreStatsResult res;
    std::istringstream in(input);
    int n;
    if(!(in >> n)){
      return res;
    }
    std::string name;
    int score;
    int sum = 0;
    int count = 0;
    int max_score = -1;
    std::string max_name;
    for(int i=0;i<n;i++){
      if(in >> name >> score) {
        sum += score;
        count++;
        if (score > max_score) {
            max_score = score;
            max_name = name;
        }
    }
  }
    if (count == 0) {
        return res;
    }
    res.top_name = max_name;
    res.top_score = max_score;
    res.avg = static_cast<double>(sum) / count;
    ok = true;
    return res;
}
    std::string SolveScoreStats(const std::string& input, bool& ok) {
    auto res = ComputeScoreStats(input, ok);
    if (!ok) {
        return {};
    }
    std::ostringstream out;
    out << "top=" << res.top_name << " " << res.top_score << "\n";
    out << "avg=" << std::fixed << std::setprecision(2) << res.avg << "\n";
    return out.str();
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
