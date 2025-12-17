#include "../../hdr/ui/ResultsWindow.h"
#include "../../hdr/core/TestResult.h"
#include "../../hdr/core/Test.h"
#include "../../hdr/utils/FileManager.h"
#include "../../hdr/utils/ColorPalette.h"
#include "../../hdr/core/Answer.h"
#include <QDir>
#include <QDateTime>
#include <QMessageBox>

ResultsWindow::ResultsWindow(const TestResult& result, const Test& test, QWidget *parent)
    : QDialog(parent), result(result), test(test) {
    setWindowTitle("Результаты теста");
    setModal(true);
    setMinimumSize(500, 400);
    setupUI();
}

ResultsWindow::~ResultsWindow() = default;

void ResultsWindow::setupUI() {
    setStyleSheet(QString("background-color: %1;").arg(ColorPalette::backgroundColor().name()));
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    QLabel* titleLabel = new QLabel("Результаты теста", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px;");
    mainLayout->addWidget(titleLabel);
    
    QLabel* testNameLabel = new QLabel("Тест: " + test.getName(), this);
    testNameLabel->setAlignment(Qt::AlignCenter);
    testNameLabel->setStyleSheet("font-size: 16px; margin: 10px;");
    mainLayout->addWidget(testNameLabel);
    
    QLabel* userNameLabel = new QLabel("Пользователь: " + result.getUserName(), this);
    userNameLabel->setAlignment(Qt::AlignCenter);
    userNameLabel->setStyleSheet("font-size: 14px; margin: 10px;");
    mainLayout->addWidget(userNameLabel);
    
    QLabel* scoreLabel = new QLabel(QString("Баллы: %1 из 10").arg(result.getScore()), this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet(
        "font-size: 20px;"
        "font-weight: bold;"
        "padding: 15px;"
        "background-color: white;"
        "border: 2px solid #ddd;"
        "border-radius: 10px;"
        "margin: 20px 0;"
    );
    mainLayout->addWidget(scoreLabel);
    
    QLabel* resultLabel = new QLabel("Результат: " + result.getResultDescription(), this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet("font-size: 16px; margin: 10px;");
    mainLayout->addWidget(resultLabel);
    
    mainLayout->addStretch();
    
    QLabel* saveInfoLabel = new QLabel("💾 Результат автоматически сохранен", this);
    saveInfoLabel->setAlignment(Qt::AlignCenter);
    saveInfoLabel->setStyleSheet("font-size: 12px; color: #666; margin: 10px;");
    mainLayout->addWidget(saveInfoLabel);
    
    QPushButton* closeButton = new QPushButton("Закрыть", this);
    closeButton->setStyleSheet(QString(
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
    ).arg(ColorPalette::addButtonColor().name())
     .arg(ColorPalette::addButtonColor().darker(110).name()));
    
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(closeButton);
    buttonsLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);
}