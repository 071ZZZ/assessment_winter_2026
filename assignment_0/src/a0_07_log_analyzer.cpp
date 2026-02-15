#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>

#include "rm_a0/a0_07_log_analyzer.hpp"
namespace rm_a0
{
  LogStats AnalyzeLogFile(const std::string& path, bool& ok) {
    LogStats res;
    std::ifstream file(path);
    if(!file.is_open()){
      ok = false;
      return {};
    }
    std::string line;
    int line_count=0;
    long long total_ms = 0;
    while(getline(file,line)){
      std::istringstream iss(line);
      std::string level;
      int ms;
      if(iss >> level >> ms){
        line_count++;
        total_ms+=ms;
        if(level=="INFO"){
          res.info++;
        }
        else if(level=="WARN"){
          res.warn++;
        }
        else if(level=="ERROR"){
          res.error++;
        }
        if(ms > res.max_ms){
          res.max_ms=ms;
          res.max_level=level;
        }
        
      }

    }
    res.avg_ms=static_cast<double>(total_ms)/line_count;
    ok=true;
    return res;
}
  std::string SolveLogAnalyzer(const std::string& input, bool& ok) {
    std::istringstream in(input);
    std::string path;
    if (!std::getline(in, path)) {
        ok = false;
        return {};
    }
    if (path.empty()) {
        ok = false;
        return "FAIL\n";
    }
    
    bool file_ok = false;
    LogStats s   = AnalyzeLogFile(path, file_ok);
    if (!file_ok) {
        ok = false;
        return "FAIL\n";
    }

    ok = true;
    std::ostringstream out;
    out << "INFO=" << s.info << "\n";
    out << "WARN=" << s.warn << "\n";
    out << "ERROR=" << s.error << "\n";
    out << "avg=" << std::fixed << std::setprecision(2) << s.avg_ms << "\n";
    out << "max=" << s.max_level << " " << s.max_ms << "\n";
    return out.str();
}
} // namespace rm_a0


int main() {
  std::string path;
  if (!std::getline(std::cin, path)) {
    return 0;
  }

  bool ok = false;
  std::string out = rm_a0::SolveLogAnalyzer(path + "\n", ok);
  std::cout << out;

  return 0;
}
