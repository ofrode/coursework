#include "../../hdr/ui/TestWindow.h"
#include "../../hdr/ui/ResultsWindow.h"
#include <algorithm>
#include <vector>
#include <QKeyEvent>

TestWindow::TestWindow(const Test& test, QWidget *parent)
    : QDialog(parent), test(test) {
    setWindowTitle("Прохождение теста: " + test.getName());
    setModal(true);
    setMinimumSize(700, 500);
    setupUI();
}

TestWindow::~TestWindow() {
    if (resultsWindow) {
        delete resultsWindow;
    }
}

void TestWindow::setupUI() {
    setStyleSheet(QString("background-color: %1;").arg(ColorPalette::backgroundColor().name()));
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    welcomeLabel = new QLabel("Добро пожаловать!\nВведите ваше имя для начала теста:", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 20px;");
    mainLayout->addWidget(welcomeLabel);
    
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Ваше имя");
    nameEdit->setStyleSheet("padding: 10px; font-size: 14px; border: 2px solid #ddd; border-radius: 5px;");
    nameEdit->setMaximumWidth(300);
    mainLayout->addStretch();
    
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addStretch();
    nameLayout->addWidget(nameEdit);
    nameLayout->addStretch();
    mainLayout->addLayout(nameLayout);
    
    startButton = new QPushButton("Начать тест", this);
    startButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    padding: 12px 30px;"
        "    border: none;"
        "    border-radius: 8px;"
        "    min-width: 200px;"
        "}"
        "QPushButton:hover { background-color: %2; }"
        "QPushButton:pressed { background-color: %3; }"
    ).arg(ColorPalette::addButtonColor().name())
     .arg(ColorPalette::addButtonColor().darker(110).name())
     .arg(ColorPalette::addButtonColor().darker(120).name()));
    
    QHBoxLayout* startLayout = new QHBoxLayout();
    startLayout->addStretch();
    startLayout->addWidget(startButton);
    startLayout->addStretch();
    mainLayout->addLayout(startLayout);
    mainLayout->addStretch();
    
    connect(startButton, &QPushButton::clicked, this, &TestWindow::onStartTestClicked);
    connect(nameEdit, &QLineEdit::returnPressed, this, &TestWindow::onStartTestClicked);
    startButton->setDefault(true);
    startButton->setAutoDefault(true);
    
    progressLabel = new QLabel("Вопрос 1 из 10", this);
    progressLabel->setAlignment(Qt::AlignCenter);
    progressLabel->setStyleSheet("font-size: 14px; margin: 10px;");
    progressLabel->setVisible(false);
    mainLayout->addWidget(progressLabel);
    
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(10);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    progressBar->setFormat("%v / %m");
    progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid #ddd;"
        "    border-radius: 5px;"
        "    text-align: center;"
        "    height: 25px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: " + ColorPalette::addButtonColor().name() + ";"
        "    border-radius: 3px;"
        "}"
    );
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);
    
    questionLabel = new QLabel("", this);
    questionLabel->setAlignment(Qt::AlignCenter);
    questionLabel->setWordWrap(true);
    questionLabel->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "padding: 30px;"
        "background-color: white;"
        "border: 2px solid #ddd;"
        "border-radius: 10px;"
        "margin: 20px 0;"
    );
    questionLabel->setVisible(false);
    mainLayout->addWidget(questionLabel);
    mainLayout->addStretch();
    
    QHBoxLayout* answersLayout = new QHBoxLayout();
    answersLayout->setContentsMargins(50, 0, 50, 0);
    
    yesButton = new QPushButton("Да", this);
    yesButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    padding: 15px 40px;"
        "    border: none;"
        "    border-radius: 8px;"
        "    min-width: 150px;"
        "}"
        "QPushButton:hover { background-color: %2; }"
    ).arg(ColorPalette::addButtonColor().name())
     .arg(ColorPalette::addButtonColor().darker(110).name()));
    
    noButton = new QPushButton("Нет", this);
    noButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    padding: 15px 40px;"
        "    border: none;"
        "    border-radius: 8px;"
        "    min-width: 150px;"
        "}"
        "QPushButton:hover { background-color: %2; }"
    ).arg(ColorPalette::deleteButtonColor().name())
     .arg(ColorPalette::deleteButtonColor().darker(110).name()));
    
    unsureButton = new QPushButton("Не могу ответить", this);
    unsureButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    padding: 15px 40px;"
        "    border: none;"
        "    border-radius: 8px;"
        "    min-width: 150px;"
        "}"
        "QPushButton:hover { background-color: %2; }"
    ).arg(ColorPalette::actionButtonColor().name())
     .arg(ColorPalette::actionButtonColor().darker(110).name()));
    
    connect(yesButton, &QPushButton::clicked, [this]() { onAnswerClicked(AnswerType::YES); });
    connect(noButton, &QPushButton::clicked, [this]() { onAnswerClicked(AnswerType::NO); });
    connect(unsureButton, &QPushButton::clicked, [this]() { onAnswerClicked(AnswerType::UNSURE); });
    
    yesButton->setAutoDefault(true);
    noButton->setAutoDefault(true);
    unsureButton->setAutoDefault(true);
    yesButton->setDefault(true);
    
    yesButton->setVisible(false);
    noButton->setVisible(false);
    unsureButton->setVisible(false);
    
    answersLayout->addWidget(yesButton);
    answersLayout->addWidget(noButton);
    answersLayout->addWidget(unsureButton);
    mainLayout->addLayout(answersLayout);
}

void TestWindow::onStartTestClicked() {
    QString userName = nameEdit->text().trimmed();
    if (userName.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, введите ваше имя!");
        return;
    }
    
    std::vector<Question> questions = test.getQuestions();
    if (questions.empty()) {
        QMessageBox::critical(this, "Ошибка", "Тест не содержит вопросов!");
        return;
    }
    
    if (questions.size() != 10) {
        QMessageBox::critical(this, "Ошибка", QString("Тест должен содержать 10 вопросов! Текущее количество: %1").arg(questions.size()));
        return;
    }
    
    progressBar->setMaximum(static_cast<int>(questions.size()));
    progressBar->setValue(0);
    
    currentResult = TestResult(test.getId(), userName, 0);
    testStarted = true;
    currentQuestionIndex = 0;
    
    welcomeLabel->setVisible(false);
    nameEdit->setVisible(false);
    startButton->setVisible(false);
    
    progressLabel->setVisible(true);
    progressBar->setVisible(true);
    questionLabel->setVisible(true);
    yesButton->setVisible(true);
    noButton->setVisible(true);
    unsureButton->setVisible(true);
    
    showQuestion();
}

void TestWindow::showQuestion() {
    std::vector<Question> questions = test.getQuestions();
    int questionsCount = static_cast<int>(questions.size());
    
    if (currentQuestionIndex < 0 || currentQuestionIndex >= questionsCount) {
        if (currentQuestionIndex >= questionsCount) {
            finishTest();
        } else {
            QMessageBox::critical(this, "Ошибка", "Неверный индекс вопроса!");
        }
        return;
    }
    
    Question question = questions[currentQuestionIndex];
    questionLabel->setText(QString("Вопрос %1:\n\n%2")
        .arg(currentQuestionIndex + 1)
        .arg(question.getText()));
    
    progressBar->setValue(currentQuestionIndex + 1);
    progressLabel->setText(QString("Вопрос %1 из %2")
        .arg(currentQuestionIndex + 1)
        .arg(questionsCount));
    
    yesButton->setFocus();
    yesButton->setDefault(true);
    noButton->setDefault(false);
    unsureButton->setDefault(false);
}

void TestWindow::onAnswerClicked(AnswerType answer) {
    std::vector<AnswerType> tempAnswers = currentResult.getAnswers();
    tempAnswers.push_back(answer);
    
    int score = 0;
    for (auto ans : tempAnswers) {
        if (ans == AnswerType::YES) {
            score++;
        }
    }
    
    TestResult newResult(test.getId(), currentResult.getUserName(), score);
    for (auto ans : tempAnswers) {
        newResult.addAnswer(ans);
    }
    currentResult = newResult;
    
    currentQuestionIndex++;
    showQuestion();
}

void TestWindow::finishTest() {
    emit testCompleted(currentResult);
    showResults();
}

void TestWindow::showResults() {
    resultsWindow = new ResultsWindow(currentResult, test, this);
    resultsWindow->exec();
    if (resultsWindow) {
        resultsWindow->deleteLater();
        resultsWindow = nullptr;
    }
    accept();
}

void TestWindow::keyPressEvent(QKeyEvent* event) {
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && testStarted) {
        if (QWidget* focusedWidget = focusWidget(); focusedWidget == yesButton || focusedWidget == noButton || focusedWidget == unsureButton) {
            QPushButton* button = qobject_cast<QPushButton*>(focusedWidget);
            if (button) {
                button->click();
                return;
            }
        }
        if (yesButton->isVisible()) {
            yesButton->click();
            return;
        }
    }
    QDialog::keyPressEvent(event);
}

