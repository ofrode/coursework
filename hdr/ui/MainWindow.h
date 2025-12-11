#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "../core/Test.h"
#include "../utils/ColorPalette.h"
#include <vector>
#include <memory>

class AddTestWindow;
class StatisticsWindow;
class TestWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onAddTestClicked();
    void onDeleteTestClicked(int testId);
    void onTakeTestClicked(int testId);
    void onViewStatisticsClicked(int testId);
    void refreshTable();

private:
    void setupUI();
    void loadTests();
    void saveTests();
    Test* findTest(int testId);
    int getNextTestId() { return nextTestId++; }
    
    QTableWidget* tableWidget = nullptr;
    QPushButton* addButton = nullptr;
    std::vector<Test> tests;  // STL контейнер
    int nextTestId = 1;  // in-class initializer
    
    // Указатели на окна (для управления жизненным циклом)
    AddTestWindow* addTestWindow = nullptr;      // in-class initializer
    StatisticsWindow* statisticsWindow = nullptr; // in-class initializer
    TestWindow* testWindow = nullptr;            // in-class initializer
};

#endif // MAINWINDOW_H

