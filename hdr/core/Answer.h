#ifndef ANSWER_H
#define ANSWER_H

#include <QString>

// Перечисление типов ответов
enum class AnswerType {
    YES = 0,      // Да
    NO = 1,       // Нет
    UNSURE = 2    // Не могу ответить
};

// Класс для хранения ответа
class Answer {
public:
    Answer() : type(AnswerType::UNSURE), score(0) {}
    Answer(AnswerType t, int s) : type(t), score(s) {}
    
    AnswerType getType() const { return type; }
    int getScore() const { return score; }
    QString getTypeString() const;
    
    // Перегрузка операторов (C++20: operator== и operator!= генерируются автоматически)
    bool operator==(const Answer& other) const = default;
    
private:
    AnswerType type;
    int score;  // Баллы за ответ
};

#endif // ANSWER_H

