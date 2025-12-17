#ifndef QUESTION_H
#define QUESTION_H

#include "Answer.h"
#include <QString>
#include <vector>

class Question {
public:
    Question() : id(0) {}
    Question(int id, const QString& text);
    
    int getId() const { return id; }
    QString getText() const { return text; }
    std::vector<Answer> getAnswers() const { return answers; }
    
    void setText(const QString& t) { text = t; }
    void addAnswer(const Answer& answer);
    
    bool operator==(const Question& other) const;
    
private:
    int id;
    QString text;
    std::vector<Answer> answers;
};

#endif

