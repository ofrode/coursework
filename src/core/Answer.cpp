#include "../../hdr/core/Answer.h"

QString Answer::getTypeString() const {
    using enum AnswerType;
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

