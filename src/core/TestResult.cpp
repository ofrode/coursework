#include "../../hdr/core/TestResult.h"
#include "../../hdr/core/Answer.h"
#include <QDateTime>

TestResult::TestResult(int testId, const QString& userName, int score)
    : testId(testId), userName(userName), score(score) {
    dateTime = QDateTime::currentDateTime();
}

void TestResult::addAnswer(AnswerType answer) {
    answers.push_back(answer);
}

QString TestResult::getResultDescription() const {
    if (score >= 8) {
        return "Высокий результат";
    } else if (score >= 5) {
        return "Средний результат";
    } else {
        return "Низкий результат";
    }
}

