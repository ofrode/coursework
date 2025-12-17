#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Test.h"
#include "Question.h"
#include "Statistics.h"
#include "TestResult.h"
#include <QString>
#include <vector>
#include <map>
#include <memory>

class DataManager {
public:
    static DataManager& getInstance();
    
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    
    void loadAllTests();
    
    void loadResultsForTest(int testId);
    
    Test* getTestById(int testId);
    
    std::vector<Test>& getAllTests();
    
    Question* getQuestion(int testId, int questionIndex);
    
    std::vector<Question> getQuestions(int testId);
    
    void collectStatisticsForTest(int testId);
    
    Statistics* getStatisticsForTest(int testId);
    
    Statistics::QuestionStats getQuestionStats(int testId, int questionId);
    
    void addTest(const Test& test);
    
    void addResultToTest(int testId, const TestResult& result);
    
    void saveTestResult(const TestResult& result);
    
    void saveStatistics(int testId);
    
    void reloadData();

private:
    static inline DataManager instance{};
    DataManager();
    ~DataManager() = default;
    
    std::vector<Test> tests;
    std::map<int, Statistics> statisticsMap;
    std::map<int, bool> statisticsLoaded;
};

#endif


