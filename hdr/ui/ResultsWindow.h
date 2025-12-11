#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "../core/TestResult.h"
#include "../core/Test.h"
#include "../utils/FileManager.h"
#include "../utils/ColorPalette.h"

class ResultsWindow : public QDialog {
    Q_OBJECT

public:
    explicit ResultsWindow(const TestResult& result, const Test& test, QWidget *parent = nullptr);
    ~ResultsWindow() override;

private slots:
    void onSaveClicked();

private:
    void setupUI();
    
    TestResult result;
    const Test& test;
};

#endif // RESULTSWINDOW_H

