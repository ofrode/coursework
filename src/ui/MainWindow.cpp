#include "../../hdr/ui/MainWindow.h"
#include "../../hdr/ui/AddTestWindow.h"
#include "../../hdr/ui/StatisticsWindow.h"
#include "../../hdr/ui/TestWindow.h"
#include "../../hdr/utils/FileManager.h"
#include "../../hdr/utils/TemplateUtils.h"
#include <QDir>
#include <QDebug>
#include <algorithm>
#include <ranges>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    loadTests();
    refreshTable();
}

MainWindow::~MainWindow() {
    saveTests();
}

void MainWindow::setupUI() {
    setWindowTitle("Психологические тесты");
    setGeometry(100, 100, 1000, 600);
    setStyleSheet(QString("background-color: %1;").arg(ColorPalette::backgroundColor().name()));
    
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Кнопка добавления теста (справа вверху)
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addStretch();
    addButton = new QPushButton("+ Добавить тест", this);
    addButton->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 8px;"
        "    min-width: 150px;"
        "}"
        "QPushButton:hover {"
        "    background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %3;"
        "}"
    ).arg(ColorPalette::addButtonColor().name())
     .arg(ColorPalette::addButtonColor().darker(110).name())
     .arg(ColorPalette::addButtonColor().darker(120).name()));
    
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddTestClicked);
    topLayout->addWidget(addButton);
    mainLayout->addLayout(topLayout);
    
    // Таблица тестов
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(4);
    QStringList headers;
    headers << "ID теста" << "Название" << "Статистика" << "Действия";
    tableWidget->setHorizontalHeaderLabels(headers);
    
    // Настройка режимов изменения размеров столбцов
    QHeaderView* header = tableWidget->horizontalHeader();
    
    // Устанавливаем минимальные размеры для всех столбцов
    header->setMinimumSectionSize(50);
    
    // ID теста - фиксированный размер по содержимому
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    
    // Название - растягивается, занимает основное пространство (будет самым широким)
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    
    // Статистика - размер по содержимому
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    
    // Действия - размер по содержимому, но не будет растягиваться (будет уже, чем Название)
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setStyleSheet(
        "QTableWidget {"
        "    background-color: white;"
        "    border: 1px solid #ddd;"
        "    border-radius: 5px;"
        "}"
        "QHeaderView::section {"
        "    background-color: " + ColorPalette::paleBlue().name() + ";"
        "    padding: 8px;"
        "    border: none;"
        "    font-weight: bold;"
        "}"
    );
    
    mainLayout->addWidget(tableWidget);
}

void MainWindow::refreshTable() {
    tableWidget->setRowCount(0);
    
    for (const auto& test : tests) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        
        // ID теста
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(test.getId()));
        idItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 0, idItem);
        
        // Название
        QTableWidgetItem* nameItem = new QTableWidgetItem(test.getName());
        tableWidget->setItem(row, 1, nameItem);
        
        // Кнопка статистики
        QPushButton* statsButton = new QPushButton("📊 Статистика");
        statsButton->setStyleSheet(QString(
            "QPushButton {"
            "    background-color: %1;"
            "    color: white;"
            "    padding: 5px 10px;"
            "    border: none;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover { background-color: %2; }"
        ).arg(ColorPalette::actionButtonColor().name())
         .arg(ColorPalette::actionButtonColor().darker(110).name()));
        
        int testId = test.getId();
        connect(statsButton, &QPushButton::clicked, [this, testId]() {
            onViewStatisticsClicked(testId);
        });
        tableWidget->setCellWidget(row, 2, statsButton);
        
        // Кнопки действий
        QWidget* actionsWidget = new QWidget();
        QHBoxLayout* actionsLayout = new QHBoxLayout(actionsWidget);
        actionsLayout->setContentsMargins(5, 2, 5, 2);
        actionsLayout->setSpacing(5);
        
        QPushButton* deleteButton = new QPushButton("🗑️ Удалить");
        deleteButton->setStyleSheet(QString(
            "QPushButton {"
            "    background-color: %1;"
            "    color: white;"
            "    padding: 5px 10px;"
            "    border: none;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover { background-color: %2; }"
        ).arg(ColorPalette::deleteButtonColor().name())
         .arg(ColorPalette::deleteButtonColor().darker(110).name()));
        
        QPushButton* takeButton = new QPushButton("▶️ Пройти");
        takeButton->setStyleSheet(QString(
            "QPushButton {"
            "    background-color: %1;"
            "    color: white;"
            "    padding: 5px 10px;"
            "    border: none;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover { background-color: %2; }"
        ).arg(ColorPalette::addButtonColor().name())
         .arg(ColorPalette::addButtonColor().darker(110).name()));
        
        connect(deleteButton, &QPushButton::clicked, [this, testId]() {
            onDeleteTestClicked(testId);
        });
        connect(takeButton, &QPushButton::clicked, [this, testId]() {
            onTakeTestClicked(testId);
        });
        
        actionsLayout->addWidget(deleteButton);
        actionsLayout->addWidget(takeButton);
        actionsLayout->addStretch();
        
        tableWidget->setCellWidget(row, 3, actionsWidget);
    }
    
    // Не вызываем resizeColumnsToContents(), так как мы используем Stretch для столбца "Название"
    // и ResizeToContents для остальных столбцов, что обеспечит правильное распределение пространства
}

void MainWindow::onAddTestClicked() {
    if (addTestWindow) {
        addTestWindow->close();
        delete addTestWindow;
    }
    
    addTestWindow = new AddTestWindow(getNextTestId(), this);
    connect(addTestWindow, &AddTestWindow::testAdded, this, [this](const Test& test) {
        tests.push_back(test);
        nextTestId = std::max(nextTestId, test.getId() + 1);
        refreshTable();
        saveTests();
    });
    connect(addTestWindow, &QDialog::finished, this, [this]() {
        addTestWindow = nullptr;
    });
    addTestWindow->show();
}

void MainWindow::onDeleteTestClicked(int testId) {
    int ret = QMessageBox::question(this, "Подтверждение", 
        "Вы уверены, что хотите удалить этот тест?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        const auto eraseRange = std::ranges::remove_if(tests,
            [testId](const Test& t) { return t.getId() == testId; });
        tests.erase(eraseRange.begin(), eraseRange.end());
        refreshTable();
        saveTests();
    }
}

void MainWindow::onTakeTestClicked(int testId) {
    const Test* test = findTest(testId);
    if (!test) return;
    
    if (testWindow) {
        testWindow->close();
        delete testWindow;
    }
    
    testWindow = new TestWindow(*test, this);
    connect(testWindow, &TestWindow::testCompleted, this, [this, testId](const TestResult& result) {
        Test* test = findTest(testId);
        if (test) {
            // Добавляем результат в тест
            test->addResult(result);
            
            // Автоматически сохраняем результат прохождения теста
            try {
                FileManager::saveResultAutomatically(result);
            } catch (const FileException& e) {
                qDebug() << "Ошибка автоматического сохранения результата:" << e.what();
            }
            
            // Автоматически сохраняем статистику (кумулятивную по всем прохождениям)
            try {
                FileManager::saveStatisticsAutomatically(*test);
            } catch (const FileException& e) {
                qDebug() << "Ошибка автоматического сохранения статистики:" << e.what();
            }
            
            // Сохраняем тест с новым результатом
            refreshTable();
            saveTests();
        }
    });
    connect(testWindow, &QDialog::finished, this, [this]() {
        testWindow = nullptr;
    });
    testWindow->show();
}

void MainWindow::onViewStatisticsClicked(int testId) {
    const Test* test = findTest(testId);
    if (!test) return;
    
    if (statisticsWindow) {
        statisticsWindow->close();
        delete statisticsWindow;
    }
    
    statisticsWindow = new StatisticsWindow(*test, this);
    connect(statisticsWindow, &QDialog::finished, this, [this]() {
        statisticsWindow = nullptr;
    });
    statisticsWindow->show();
}

void MainWindow::loadTests() {
    try {
        QString testDir = FileManager::getTestQuestionDir();
        tests = FileManager::loadAllTests(testDir);
        
        // Найти максимальный ID
        for (const auto& test : tests) {
            if (test.getId() >= nextTestId) {
                nextTestId = test.getId() + 1;
            }
        }
        
        // Загружаем все результаты для каждого теста из TestAnswerTxt
        for (auto& test : tests) {
            loadResultsForTest(test);
        }
        
        // После загрузки всех результатов автоматически сохраняем статистику для каждого теста
        for (const auto& test : tests) {
            if (!test.getResults().empty()) {
                saveStatisticsForTest(test);
            }
        }
    } catch (const FileException& e) {
        qDebug() << "Ошибка загрузки тестов:" << e.what();
    }
}

void MainWindow::saveTests() const {
    QString testDir = FileManager::getTestQuestionDir();
    
    for (const auto& test : tests) {
        try {
            QDir dir(testDir);
            QString filename = dir.absoluteFilePath(QString("test_%1.txt").arg(test.getId()));
            FileManager::saveTestToFile(test, filename);
        } catch (const FileException& e) {
            qDebug() << "Ошибка сохранения теста:" << e.what();
        }
    }
}

void MainWindow::loadResultsForTest(Test& test) const {
    try {
        std::vector<TestResult> results = FileManager::loadAllResultsForTest(test.getId());
        // Добавляем все загруженные результаты в тест
        for (const auto& result : results) {
            test.addResult(result);
        }
    } catch (const FileException& e) {
        qDebug() << "Ошибка загрузки результатов для теста" << test.getId() << ":" << e.what();
    }
}

void MainWindow::saveStatisticsForTest(const Test& test) const {
    try {
        FileManager::saveStatisticsAutomatically(test);
    } catch (const FileException& e) {
        qDebug() << "Ошибка сохранения статистики для теста" << test.getId() << ":" << e.what();
    }
}

Test* MainWindow::findTest(int testId) {
    for (auto& test : tests) {
        if (test.getId() == testId) {
            return &test;
        }
    }
    return nullptr;
}

const Test* MainWindow::findTest(int testId) const {
    for (const auto& test : tests) {
        if (test.getId() == testId) {
            return &test;
        }
    }
    return nullptr;
}

