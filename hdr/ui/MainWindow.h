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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    
    QTableWidget* tableWidget;
    QPushButton* addButton;
    std::vector<Test> tests;  // STL контейнер
    int nextTestId;
    
    // Указатели на окна (для управления жизненным циклом)
    AddTestWindow* addTestWindow;
    StatisticsWindow* statisticsWindow;
    TestWindow* testWindow;
};

#endif // MAINWINDOW_H

