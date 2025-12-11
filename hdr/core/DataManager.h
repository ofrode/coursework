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

// Класс-менеджер данных, связывающий Question и Statistics
// Промежуточный класс для управления тестами, вопросами и статистикой
class DataManager {
public:
    // Singleton паттерн для единой точки доступа
    static DataManager& getInstance();
    
    // Запрет копирования
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    
    // Загрузка всех тестов
    void loadAllTests();
    
    // Загрузка всех результатов для теста
    void loadResultsForTest(int testId);
    
    // Получение теста по ID
    Test* getTestById(int testId);
    
    // Получение всех тестов
    std::vector<Test>& getAllTests();
    
    // Получение вопроса из теста
    Question* getQuestion(int testId, int questionIndex);
    
    // Получение всех вопросов теста
    std::vector<Question> getQuestions(int testId);
    
    // Сбор статистики по тесту
    void collectStatisticsForTest(int testId);
    
    // Получение статистики по тесту
    Statistics* getStatisticsForTest(int testId);
    
    // Получение статистики по вопросу
    Statistics::QuestionStats getQuestionStats(int testId, int questionId);
    
    // Добавление нового теста
    void addTest(const Test& test);
    
    // Добавление результата к тесту
    void addResultToTest(int testId, const TestResult& result);
    
    // Сохранение результата теста в файл
    void saveTestResult(const TestResult& result);
    
    // Сохранение статистики теста в файл
    void saveStatistics(int testId);
    
    // Перезагрузка данных
    void reloadData();

private:
    DataManager();
    ~DataManager() = default;
    
    std::vector<Test> tests;                        // Все загруженные тесты
    std::map<int, Statistics> statisticsMap;        // Статистика по ID теста
    std::map<int, bool> statisticsLoaded;           // Флаг загрузки статистики
};

#endif // DATAMANAGER_H


