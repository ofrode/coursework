#ifndef TEST_H
#define TEST_H

#include "Question.h"
#include "TestResult.h"
#include <QString>
#include <vector>
#include <memory>


class BaseTest {
public:
    virtual ~BaseTest() = default;
    virtual QString getType() const = 0;
};

class Test : public BaseTest {
public:
    Test();
    Test(int id, const QString& name);
    ~Test() override;
    
    int getId() const { return id; }
    QString getName() const { return name; }
    std::vector<Question> getQuestions() const { return questions; }
    std::vector<TestResult> getResults() const { return results; }
    
    void setName(const QString& n) { name = n; }
    void addQuestion(const Question& question);
    void addResult(const TestResult& result);
    void removeQuestion(int index);
    
    bool operator==(const Test& other) const;
    Test& operator=(const Test& other);
    
    friend QString testToString(const Test& test);

    QString getType() const override { return "Психологический тест"; }
    
private:
    int id;
    QString name;
    std::vector<Question> questions;
    std::vector<TestResult> results;
};

QString testToString(const Test& test);

#endif

