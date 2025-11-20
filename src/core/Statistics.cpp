#include "../../hdr/core/Statistics.h"
#include "../../hdr/core/Test.h"
#include "../../hdr/core/TestResult.h"
#include "../../hdr/core/Answer.h"

Statistics::Statistics() : totalTests(0), totalAnswers(0) {}

void Statistics::collectStatistics(const Test& test) {
    totalTests++;
    
    for (const auto& result : test.getResults()) {
        const auto& answers = result.getAnswers();
        totalAnswers += answers.size();
        
        for (size_t i = 0; i < answers.size(); ++i) {
            int questionId = static_cast<int>(i);
            
            if (questionStats.find(questionId) == questionStats.end()) {
                QuestionStats stats;
                stats.totalAnswers = 0;
                stats.yesCount = 0;
                stats.noCount = 0;
                stats.unsureCount = 0;
                questionStats[questionId] = stats;
            }
            
            QuestionStats& stats = questionStats[questionId];
            stats.totalAnswers++;
            
            switch(answers[i]) {
                case AnswerType::YES:
                    stats.yesCount++;
                    break;
                case AnswerType::NO:
                    stats.noCount++;
                    break;
                case AnswerType::UNSURE:
                    stats.unsureCount++;
                    break;
            }
        }
    }
    
    // Вычисление процентов
    for (auto& pair : questionStats) {
        QuestionStats& stats = pair.second;
        if (stats.totalAnswers > 0) {
            stats.yesPercent = (stats.yesCount * 100.0) / stats.totalAnswers;
            stats.noPercent = (stats.noCount * 100.0) / stats.totalAnswers;
            stats.unsurePercent = (stats.unsureCount * 100.0) / stats.totalAnswers;
        }
    }
}

Statistics::QuestionStats Statistics::getQuestionStats(int questionId) const {
    auto it = questionStats.find(questionId);
    if (it != questionStats.end()) {
        return it->second;
    }
    return QuestionStats();
}

