#include "../../hdr/core/Question.h"
#include "../../hdr/core/Answer.h"

Question::Question(int id, const QString& text) : id(id), text(text) {
    using enum AnswerType;  // C++20: сокращение записи
    // Инициализация стандартных ответов
    answers.push_back(Answer(YES, 1));
    answers.push_back(Answer(NO, 0));
    answers.push_back(Answer(UNSURE, 0));
}

void Question::addAnswer(const Answer& answer) {
    answers.push_back(answer);
}

bool Question::operator==(const Question& other) const {
    return id == other.id && text == other.text;
}
// C++20: operator!= генерируется автоматически из operator==

