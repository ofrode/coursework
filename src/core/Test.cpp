#include "../../hdr/core/Test.h"
#include "../../hdr/core/Question.h"
#include "../../hdr/core/TestResult.h"

Test::Test() : id(0) {}

Test::Test(int id, const QString& name) : id(id), name(name) {}

void Test::addQuestion(const Question& question) {
    questions.push_back(question);
}

void Test::addResult(const TestResult& result) {
    results.push_back(result);
}

void Test::removeQuestion(int index) {
    if (index >= 0 && index < static_cast<int>(questions.size())) {
        questions.erase(questions.begin() + index);
    }
}

bool Test::operator==(const Test& other) const {
    return id == other.id && name == other.name;
}
// C++20: operator!= генерируется автоматически

Test& Test::operator=(const Test& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
        questions = other.questions;
        results = other.results;
    }
    return *this;
}

// Дружественная функция
QString testToString(const Test& test) {
    return QString("Тест #%1: %2 (Вопросов: %3)")
        .arg(test.id)
        .arg(test.name)
        .arg(test.questions.size());
}

