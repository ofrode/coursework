#ifndef ADDTESTWINDOW_H
#define ADDTESTWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QListWidget>
#include "../core/Test.h"
#include "../core/Question.h"
#include "../utils/FileManager.h"
#include "../utils/ColorPalette.h"

class AddTestWindow : public QDialog {
    Q_OBJECT

public:
    explicit AddTestWindow(int testId, QWidget *parent = nullptr);
    explicit AddTestWindow(QWidget *parent = nullptr);
    ~AddTestWindow();

signals:
    void testAdded(const Test& test);

private slots:
    void onAddFromFileClicked();
    void onAddQuestionClicked();
    void onRemoveQuestionClicked();
    void onSaveTestClicked();

private:
    void setupUI();
    void clearForm();
    bool validateTest();
    
    QLineEdit* nameEdit;
    QListWidget* questionsList;
    QTextEdit* questionTextEdit;
    QPushButton* addQuestionButton;
    QPushButton* removeQuestionButton;
    QPushButton* addFromFileButton;
    QPushButton* saveButton;
    QPushButton* cancelButton;
    
    std::vector<QString> questions;  // STL контейнер
    int nextQuestionId;
    int testId;
};

#endif // ADDTESTWINDOW_H

