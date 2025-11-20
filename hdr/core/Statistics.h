#ifndef STATISTICS_H
#define STATISTICS_H

#include "Test.h"
#include <QString>
#include <map>
#include <vector>

// Класс для сбора статистики
class Statistics {
public:
    Statistics();
    
    // Сбор статистики по тесту
    void collectStatistics(const Test& test);
    
    // Получение статистики по вопросу
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
    
    // Общая статистика
    int getTotalTests() const { return totalTests; }
    int getTotalAnswers() const { return totalAnswers; }
    
private:
    std::map<int, QuestionStats> questionStats;  // STL контейнер
    int totalTests;
    int totalAnswers;
};

#endif // STATISTICS_H

