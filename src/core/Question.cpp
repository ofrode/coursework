#include "../../hdr/core/Question.h"
#include "../../hdr/core/Answer.h"

Question::Question(int id, const QString& text) : id(id), text(text) {
    // Инициализация стандартных ответов
    answers.push_back(Answer(AnswerType::YES, 1));
    answers.push_back(Answer(AnswerType::NO, 0));
    answers.push_back(Answer(AnswerType::UNSURE, 0));
}

void Question::addAnswer(const Answer& answer) {
    answers.push_back(answer);
}

bool Question::operator==(const Question& other) const {
    return id == other.id && text == other.text;
}

bool Question::operator!=(const Question& other) const {
    return !(*this == other);
}

