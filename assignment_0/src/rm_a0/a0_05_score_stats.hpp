#ifndef a0_05_score_stats_hpp
#define a0_05_score_stats_hpp
#include <string>
namespace rm_a0{
    struct ScoreStatsResult {
    std::string top_name;  
    int top_score = 0;
    double avg = 0.0;
};
    ScoreStatsResult ComputeScoreStats(const std::string& input, bool& ok);
    std::string SolveScoreStats(const std::string& input, bool& ok);
}
#endif 