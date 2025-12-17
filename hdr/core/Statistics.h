#ifndef STATISTICS_H
#define STATISTICS_H

#include "Test.h"
#include <QString>
#include <map>
#include <vector>

class Statistics {
public:
    Statistics();
    
    void collectStatistics(const Test& test);
    
    struct QuestionStats {
        int totalAnswers;
        int yesCount;
        int noCount;
        int unsureCount;
        double yesPercent;
        double noPercent;
        double unsurePercent;
    };
    
    QuestionStats getQuestionStats(int questionId) const;
    std::map<int, QuestionStats> getAllStats() const { return questionStats; }
    
    int getTotalTests() const { return totalTests; }
    int getTotalAnswers() const { return totalAnswers; }
    
private:
    std::map<int, QuestionStats> questionStats;
    int totalTests = 0;
    int totalAnswers = 0;
};

#endif

