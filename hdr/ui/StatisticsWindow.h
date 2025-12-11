#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include "../core/Test.h"
#include "../core/Statistics.h"
#include "../utils/ColorPalette.h"
#include <QPainter>
#include <QWidget>

// Виджет для отображения диаграммы
class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);
    void setData(double yes, double no, double unsure);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    void drawLegend(QPainter& painter, int x, int y, int chartWidth) const;
    
    double yesPercent = 0;     // in-class initializer
    double noPercent = 0;      // in-class initializer
    double unsurePercent = 0;  // in-class initializer
};

class StatisticsWindow : public QDialog {
    Q_OBJECT

public:
    explicit StatisticsWindow(const Test& test, QWidget *parent = nullptr);
    ~StatisticsWindow() override;

private:
    void setupUI();
    void updateStatistics();
    
    const Test& test;
    Statistics statistics;
    QTableWidget* statsTable;
    ChartWidget* chartWidget;
};

#endif // STATISTICSWINDOW_H

