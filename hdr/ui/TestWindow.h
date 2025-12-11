#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QMessageBox>
#include <QKeyEvent>
#include "../core/Test.h"
#include "../core/TestResult.h"
#include "../utils/FileManager.h"
#include "../utils/ColorPalette.h"
#include "../core/Answer.h"

class ResultsWindow;

class TestWindow : public QDialog {
    Q_OBJECT

public:
    explicit TestWindow(const Test& test, QWidget *parent = nullptr);
    ~TestWindow() override;
    
protected:
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void testCompleted(const TestResult& result);

private slots:
    void onStartTestClicked();
    void onAnswerClicked(AnswerType answer);

private:
    void setupUI();
    void showQuestion();
    void finishTest();
    void showResults();
    
    const Test& test;
    TestResult currentResult;
    int currentQuestionIndex;
    bool testStarted;
    
    QLabel* welcomeLabel;
    QLineEdit* nameEdit;
    QPushButton* startButton;
    QLabel* questionLabel;
    QProgressBar* progressBar;
    QPushButton* yesButton;
    QPushButton* noButton;
    QPushButton* unsureButton;
    QLabel* progressLabel;
    
    ResultsWindow* resultsWindow;
};

#endif // TESTWINDOW_H

