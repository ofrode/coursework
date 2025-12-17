#ifndef ANSWER_H
#define ANSWER_H

#include <QString>

enum class AnswerType {
    YES = 0, 
    NO = 1,
    UNSURE = 2 
};

class Answer {
public:
    Answer() : type(AnswerType::UNSURE), score(0) {}
    Answer(AnswerType t, int s) : type(t), score(s) {}
    
    AnswerType getType() const { return type; }
    int getScore() const { return score; }
    QString getTypeString() const;
    
    bool operator==(const Answer& other) const = default;
    
private:
    AnswerType type;
    int score;
};

#endif
