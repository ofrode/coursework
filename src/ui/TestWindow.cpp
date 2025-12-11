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
    
    // Приветствие и ввод имени
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
    // Подключаем Enter в поле ввода имени к кнопке "Начать тест"
    connect(nameEdit, &QLineEdit::returnPressed, this, &TestWindow::onStartTestClicked);
    // Устанавливаем кнопку "Начать тест" как default, чтобы она активировалась по Enter
    startButton->setDefault(true);
    startButton->setAutoDefault(true);
    
    // Элементы для прохождения теста (скрыты изначально)
    progressLabel = new QLabel("Вопрос 1 из 10", this);
    progressLabel->setAlignment(Qt::AlignCenter);
    progressLabel->setStyleSheet("font-size: 14px; margin: 10px;");
    progressLabel->setVisible(false);
    mainLayout->addWidget(progressLabel);
    
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    // Максимум будет установлен динамически при запуске теста
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
    
    // Кнопки ответов
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
    
    // Устанавливаем кнопки ответов как autoDefault, чтобы они активировались по Enter
    // setAutoDefault(true) автоматически устанавливает кнопку с фокусом как default при переключении Tab
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
    
    // Проверка на наличие вопросов
    std::vector<Question> questions = test.getQuestions();
    if (questions.empty()) {
        QMessageBox::critical(this, "Ошибка", "Тест не содержит вопросов!");
        return;
    }
    
    if (questions.size() != 10) {
        QMessageBox::critical(this, "Ошибка", QString("Тест должен содержать 10 вопросов! Текущее количество: %1").arg(questions.size()));
        return;
    }
    
    // Инициализация прогресс-бара с правильным количеством вопросов
    progressBar->setMaximum(static_cast<int>(questions.size()));
    progressBar->setValue(0);
    
    currentResult = TestResult(test.getId(), userName, 0);
    testStarted = true;
    currentQuestionIndex = 0;
    
    // Скрыть элементы приветствия
    welcomeLabel->setVisible(false);
    nameEdit->setVisible(false);
    startButton->setVisible(false);
    
    // Показать элементы теста
    progressLabel->setVisible(true);
    progressBar->setVisible(true);
    questionLabel->setVisible(true);
    yesButton->setVisible(true);
    noButton->setVisible(true);
    unsureButton->setVisible(true);
    
    showQuestion();
}

void TestWindow::showQuestion() {
    // Получаем список вопросов один раз, чтобы избежать проблем с временными объектами
    std::vector<Question> questions = test.getQuestions();
    int questionsCount = static_cast<int>(questions.size());
    
    // Проверка на валидность индекса и завершение теста
    if (currentQuestionIndex < 0 || currentQuestionIndex >= questionsCount) {
        if (currentQuestionIndex >= questionsCount) {
            finishTest();
        } else {
            QMessageBox::critical(this, "Ошибка", "Неверный индекс вопроса!");
        }
        return;
    }
    
    // Используем копию вопроса вместо ссылки на временный объект
    Question question = questions[currentQuestionIndex];
    questionLabel->setText(QString("Вопрос %1:\n\n%2")
        .arg(currentQuestionIndex + 1)
        .arg(question.getText()));
    
    progressBar->setValue(currentQuestionIndex + 1);
    progressLabel->setText(QString("Вопрос %1 из %2")
        .arg(currentQuestionIndex + 1)
        .arg(questionsCount));
    
    // Устанавливаем фокус на первую кнопку (Да), чтобы можно было нажать Enter
    yesButton->setFocus();
    yesButton->setDefault(true);
    noButton->setDefault(false);
    unsureButton->setDefault(false);
}

void TestWindow::onAnswerClicked(AnswerType answer) {
    // Сохранить ответы во временный вектор
    std::vector<AnswerType> tempAnswers = currentResult.getAnswers();
    tempAnswers.push_back(answer);
    
    // Подсчет баллов
    int score = 0;
    for (auto ans : tempAnswers) {
        if (ans == AnswerType::YES) {
            score++;
        }
    }
    
    // Создать новый результат с правильными данными
    TestResult newResult(test.getId(), currentResult.getUserName(), score);
    for (auto ans : tempAnswers) {
        newResult.addAnswer(ans);
    }
    currentResult = newResult;
    
    currentQuestionIndex++;
    showQuestion();
}

void TestWindow::finishTest() {
    // Результат уже готов, просто показать его
    emit testCompleted(currentResult);
    showResults();
}

void TestWindow::showResults() {
    resultsWindow = new ResultsWindow(currentResult, test, this);
    // Показываем окно результатов модально
    resultsWindow->exec();
    // После закрытия окна результатов закрываем TestWindow
    if (resultsWindow) {
        resultsWindow->deleteLater();
        resultsWindow = nullptr;
    }
    accept(); // Закрываем TestWindow
}

void TestWindow::keyPressEvent(QKeyEvent* event) {
    // Если нажат Enter и тест запущен, активируем кнопку с фокусом
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && testStarted) {
        // Находим кнопку с фокусом и активируем её
        QWidget* focusedWidget = focusWidget();
        if (focusedWidget == yesButton || focusedWidget == noButton || focusedWidget == unsureButton) {
            QPushButton* button = qobject_cast<QPushButton*>(focusedWidget);
            if (button) {
                button->click();
                return;
            }
        }
        // Если фокус не на кнопке, но кнопки видны, активируем первую кнопку (Да)
        if (yesButton->isVisible()) {
            yesButton->click();
            return;
        }
    }
    // Вызываем базовую реализацию для обработки других клавиш
    QDialog::keyPressEvent(event);
}

