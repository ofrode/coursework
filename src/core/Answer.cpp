#include "../../hdr/core/Answer.h"

QString Answer::getTypeString() const {
    switch(type) {
        case AnswerType::YES:
            return "Да";
        case AnswerType::NO:
            return "Нет";
        case AnswerType::UNSURE:
            return "Не могу ответить";
        default:
            return "Неизвестно";
    }
}

bool Answer::operator==(const Answer& other) const {
    return type == other.type && score == other.score;
}
// C++20: operator!= генерируется автоматически из operator==

