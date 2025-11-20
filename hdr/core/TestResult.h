#ifndef TESTRESULT_H
#define TESTRESULT_H

#include <QString>
#include <QDateTime>
#include <vector>
#include "Answer.h"

// Класс для хранения результата теста
class TestResult {
public:
    TestResult() : testId(0), score(0) {}
    TestResult(int testId, const QString& userName, int score);
    
    int getTestId() const { return testId; }
    QString getUserName() const { return userName; }
    int getScore() const { return score; }
    QString getResultText() const { return resultText; }
    QDateTime getDateTime() const { return dateTime; }
    const std::vector<AnswerType>& getAnswers() const { return answers; }
    
    void setResultText(const QString& text) { resultText = text; }
    void setDateTime(const QDateTime& dt) { dateTime = dt; }
    void addAnswer(AnswerType answer);
    QString getResultDescription() const;
    
    
private:
    int testId;
    QString userName;
    int score;
    QString resultText;
    QDateTime dateTime;
    std::vector<AnswerType> answers;  // STL контейнер
};

#endif // TESTRESULT_H

