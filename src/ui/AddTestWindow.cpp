#include "../../hdr/ui/AddTestWindow.h"
#include "../../hdr/core/Question.h"
#include "../../hdr/core/Test.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <algorithm>

AddTestWindow::AddTestWindow(int testId, QWidget *parent)
    : QDialog(parent), nextQuestionId(0), testId(testId) {
    setWindowTitle("Добавить тест");
    setModal(true);
    setMinimumSize(600, 500);
    setupUI();
}

AddTestWindow::AddTestWindow(QWidget *parent)
    : QDialog(parent), nextQuestionId(0), testId(0) {
    setWindowTitle("Добавить тест");
    setModal(true);
    setMinimumSize(600, 500);
    setupUI();
}

AddTestWindow::~AddTestWindow() {}

void AddTestWindow::setupUI() {
    setStyleSheet(QString("background-color: %1;").arg(ColorPalette::backgroundColor().name()));
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Название теста
    QLabel* nameLabel = new QLabel("Название теста:", this);
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Введите название теста");
    nameEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 5px;");
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(nameEdit);
    
    // Список вопросов
    QLabel* questionsLabel = new QLabel("Вопросы:", this);
    questionsList = new QListWidget(this);
    questionsList->setStyleSheet(
        "QListWidget {"
        "    border: 1px solid #ddd;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
    );
    mainLayout->addWidget(questionsLabel);
    mainLayout->addWidget(questionsList);
    
    // Редактирование вопроса
    QHBoxLayout* questionEditLayout = new QHBoxLayout();
    questionTextEdit = new QTextEdit(this);
    questionTextEdit->setPlaceholderText("Введите текст вопроса");
    questionTextEdit->setMaximumHeight(80);
    questionTextEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 5px;");
    questionEditLayout->addWidget(questionTextEdit);
    
    QVBoxLayout* buttonsLayout = new QVBoxLayout();
    addQuestionButton = new QPushButton("+ Добавить вопрос", this);
    addQuestionButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    padding: 8px;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: %2; }"
    ).arg(ColorPalette::addButtonColor().name())
     .arg(ColorPalette::addButtonColor().darker(110).name()));
    
    removeQuestionButton = new QPushButton("🗑️ Удалить", this);
    removeQuestionButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    padding: 8px;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: %2; }"
    ).arg(ColorPalette::deleteButtonColor().name())
     .arg(ColorPalette::deleteButtonColor().darker(110).name()));
    
    connect(addQuestionButton, &QPushButton::clicked, this, &AddTestWindow::onAddQuestionClicked);
    connect(removeQuestionButton, &QPushButton::clicked, this, &AddTestWindow::onRemoveQuestionClicked);
    
    buttonsLayout->addWidget(addQuestionButton);
    buttonsLayout->addWidget(removeQuestionButton);
    buttonsLayout->addStretch();
    questionEditLayout->addLayout(buttonsLayout);
    mainLayout->addLayout(questionEditLayout);
    
    // Кнопки действий
    QHBoxLayout* actionLayout = new QHBoxLayout();
    
    addFromFileButton = new QPushButton("📁 Загрузить из файла", this);
    addFromFileButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    padding: 10px 15px;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: %2; }"
    ).arg(ColorPalette::actionButtonColor().name())
     .arg(ColorPalette::actionButtonColor().darker(110).name()));
    
    saveButton = new QPushButton("💾 Сохранить тест", this);
    saveButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    padding: 10px 15px;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: %2; }"
    ).arg(ColorPalette::addButtonColor().name())
     .arg(ColorPalette::addButtonColor().darker(110).name()));
    
    cancelButton = new QPushButton("Отмена", this);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #ccc;"
        "    color: black;"
        "    padding: 10px 15px;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #bbb; }"
    );
    
    connect(addFromFileButton, &QPushButton::clicked, this, &AddTestWindow::onAddFromFileClicked);
    connect(saveButton, &QPushButton::clicked, this, &AddTestWindow::onSaveTestClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    actionLayout->addWidget(addFromFileButton);
    actionLayout->addStretch();
    actionLayout->addWidget(saveButton);
    actionLayout->addWidget(cancelButton);
    mainLayout->addLayout(actionLayout);
}

void AddTestWindow::onAddQuestionClicked() {
    QString text = questionTextEdit->toPlainText().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите текст вопроса!");
        return;
    }
    
    if (questions.size() >= 10) {
        QMessageBox::warning(this, "Предупреждение", "Тест может содержать максимум 10 вопросов!");
        return;
    }
    
    questions.push_back(text);
    questionsList->addItem(QString("%1. %2").arg(questions.size()).arg(text));
    questionTextEdit->clear();
}

void AddTestWindow::onRemoveQuestionClicked() {
    int row = questionsList->currentRow();
    if (row >= 0 && row < static_cast<int>(questions.size())) {
        questions.erase(questions.begin() + row);
        delete questionsList->takeItem(row);
        
        // Обновить нумерацию
        questionsList->clear();
        for (size_t i = 0; i < questions.size(); ++i) {
            questionsList->addItem(QString("%1. %2").arg(i + 1).arg(questions[i]));
        }
    }
}

void AddTestWindow::onAddFromFileClicked() {
    QString filename = QFileDialog::getOpenFileName(this, 
        "Выберите файл с тестом", "", "Text Files (*.txt);;All Files (*)");
    
    if (filename.isEmpty()) return;
    
    try {
        Test test = FileManager::loadTestFromFile(filename);
        nameEdit->setText(test.getName());
        questions.clear();
        questionsList->clear();
        
        for (const auto& question : test.getQuestions()) {
            questions.push_back(question.getText());
            questionsList->addItem(QString("%1. %2").arg(questions.size()).arg(question.getText()));
        }
        
        QMessageBox::information(this, "Успех", "Тест загружен из файла!");
    } catch (const FileException& e) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить тест:\n" + QString(e.what()));
    }
}

void AddTestWindow::onSaveTestClicked() {
    if (!validateTest()) return;
    
    // Создать тест
    Test test(testId, nameEdit->text());
    
    for (size_t i = 0; i < questions.size(); ++i) {
        Question q(static_cast<int>(i), questions[i]);
        test.addQuestion(q);
    }
    
    emit testAdded(test);
    accept();
}

bool AddTestWindow::validateTest() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите название теста!");
        return false;
    }
    
    if (questions.size() != 10) {
        QMessageBox::warning(this, "Предупреждение", "Тест должен содержать ровно 10 вопросов!");
        return false;
    }
    
    return true;
}

void AddTestWindow::clearForm() {
    nameEdit->clear();
    questions.clear();
    questionsList->clear();
    questionTextEdit->clear();
}

