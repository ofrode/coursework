#include "../../hdr/core/DataManager.h"
#include "../../hdr/utils/FileManager.h"
#include <algorithm>

DataManager& DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

DataManager::DataManager() {
}

void DataManager::loadAllTests() {
    QString testDir = FileManager::getTestQuestionDir();
    tests = FileManager::loadAllTests(testDir);
}

void DataManager::loadResultsForTest(int testId) {
    Test* test = getTestById(testId);
    if (test) {
        std::vector<TestResult> results = FileManager::loadAllResultsForTest(testId);
        for (const auto& result : results) {
            test->addResult(result);
        }
    }
}

Test* DataManager::getTestById(int testId) {
    for (auto& test : tests) {
        if (test.getId() == testId) {
            return &test;
        }
    }
    return nullptr;
}

std::vector<Test>& DataManager::getAllTests() {
    return tests;
}

Question* DataManager::getQuestion(int testId, int questionIndex) {
    Test* test = getTestById(testId);
    if (test) {
        auto questions = test->getQuestions();
        if (questionIndex >= 0 && questionIndex < static_cast<int>(questions.size())) {
            return nullptr;
        }
    }
    return nullptr;
}

std::vector<Question> DataManager::getQuestions(int testId) {
    Test* test = getTestById(testId);
    if (test) {
        return test->getQuestions();
    }
    return std::vector<Question>();
}

void DataManager::collectStatisticsForTest(int testId) {
    Test* test = getTestById(testId);
    if (test) {
        if (test->getResults().empty()) {
            loadResultsForTest(testId);
        }
        
        Statistics stats;
        stats.collectStatistics(*test);
        statisticsMap[testId] = stats;
        statisticsLoaded[testId] = true;
    }
}

Statistics* DataManager::getStatisticsForTest(int testId) {
    if (statisticsLoaded.find(testId) == statisticsLoaded.end() || !statisticsLoaded[testId]) {
        collectStatisticsForTest(testId);
    }
    
    auto it = statisticsMap.find(testId);
    if (it != statisticsMap.end()) {
        return &(it->second);
    }
    return nullptr;
}

Statistics::QuestionStats DataManager::getQuestionStats(int testId, int questionId) {
    Statistics* stats = getStatisticsForTest(testId);
    if (stats) {
        return stats->getQuestionStats(questionId);
    }
    return Statistics::QuestionStats();
}

void DataManager::addTest(const Test& test) {
    tests.push_back(test);
}

void DataManager::addResultToTest(int testId, const TestResult& result) {
    Test* test = getTestById(testId);
    if (test) {
        test->addResult(result);
        statisticsLoaded[testId] = false;
    }
}

void DataManager::saveTestResult(const TestResult& result) {
    FileManager::saveResultAutomatically(result);
    

    addResultToTest(result.getTestId(), result);
}

void DataManager::saveStatistics(int testId) {
    Test* test = getTestById(testId);
    if (test) {
        FileManager::saveStatisticsAutomatically(*test);
    }
}

void DataManager::reloadData() {
    tests.clear();
    statisticsMap.clear();
    statisticsLoaded.clear();
    loadAllTests();
}