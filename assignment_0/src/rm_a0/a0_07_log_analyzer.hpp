#ifndef a0_07_log_analyzer_hpp
#define a0_07_log_analyzer_hpp
#include <string>
namespace rm_a0
{ 
struct LogStats {
  long long info = 0;
  long long warn = 0;
  long long error = 0;
  double avg_ms = 0.0;
  std::string max_level;
  long long max_ms = 0;
};
LogStats AnalyzeLogFile(const std::string& path, bool& ok);
std::string SolveLogAnalyzer(const std::string& input, bool& ok);
} // namespace rm_a0
#endif