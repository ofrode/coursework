#ifndef TEST_H
#define TEST_H

#include "Question.h"
#include "TestResult.h"
#include <QString>
#include <vector>
#include <memory>

// Базовый класс теста
class BaseTest {
public:
    virtual ~BaseTest() = default;
    virtual QString getType() const = 0;
};

// Класс психологического теста
class Test : public BaseTest {
public:
    Test();
    Test(int id, const QString& name);
    
    int getId() const { return id; }
    QString getName() const { return name; }
    std::vector<Question> getQuestions() const { return questions; }
    std::vector<TestResult> getResults() const { return results; }
    
    void setName(const QString& n) { name = n; }
    void addQuestion(const Question& question);
    void addResult(const TestResult& result);
    void removeQuestion(int index);
    
    // Перегрузка операторов
    bool operator==(const Test& other) const;
    bool operator!=(const Test& other) const;
    Test& operator=(const Test& other);
    
    // Дружественная функция для вывода
    friend QString testToString(const Test& test);
    
    // Наследование
    QString getType() const override { return "Психологический тест"; }
    
private:
    int id;
    QString name;
    std::vector<Question> questions;  // STL контейнер
    std::vector<TestResult> results;   // STL контейнер для результатов
};

// Дружественная функция
QString testToString(const Test& test);

#endif // TEST_H

