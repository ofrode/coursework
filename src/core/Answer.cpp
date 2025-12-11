#include "../../hdr/core/Answer.h"

QString Answer::getTypeString() const {
    using enum AnswerType;  // C++20: сокращение записи
    switch(type) {
        case YES:
            return "Да";
        case NO:
            return "Нет";
        case UNSURE:
            return "Не могу ответить";
        default:
            return "Неизвестно";
    }
}

// C++20: operator== и operator!= используют = default

