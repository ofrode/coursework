#include "../../hdr/ui/StatisticsWindow.h"
#include "../../hdr/core/Test.h"
#include "../../hdr/core/Statistics.h"
#include "../../hdr/utils/ColorPalette.h"
#include "../../hdr/core/Answer.h"
#include "../../hdr/utils/FileManager.h"
#include <QHeaderView>
#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QList>
#include <QPair>
#include <cmath>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent), yesPercent(0), noPercent(0), unsurePercent(0) {
    // Увеличиваем минимальную высоту, чтобы поместилась диаграмма и легенда
    setMinimumHeight(280);
}

void ChartWidget::setData(double yesPercent, double noPercent, double unsurePercent) {
    this->yesPercent = yesPercent;
    this->noPercent = noPercent;
    this->unsurePercent = unsurePercent;
    update();
}

void ChartWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int width = this->width();
    int height = this->height();
    int margin = 20;
    
    // Выделяем место для легенды (примерно 80 пикселей снизу)
    int legendHeight = 90;
    int chartHeight = height - margin * 2 - legendHeight;
    int chartWidth = qMin(chartHeight, width - 2 * margin);
    
    // Центрируем диаграмму по горизонтали
    int chartX = (width - chartWidth) / 2;
    int chartY = margin;
    
    // Рисуем круговую диаграмму
    QRectF rect(chartX, chartY, chartWidth, chartWidth);
    
    double total = yesPercent + noPercent + unsurePercent;
    if (total == 0) {
        painter.setBrush(QBrush(ColorPalette::lightPeach()));
        painter.drawEllipse(rect);
        // Рисуем легенду даже если нет данных
        drawLegend(painter, chartX, chartY + chartWidth + 15, chartWidth);
        return;
    }
    
    double startAngle = 0;
    
    // Да (зеленый)
    if (yesPercent > 0) {
        double spanAngle = (yesPercent / total) * 360 * 16;
        painter.setBrush(QBrush(ColorPalette::addButtonColor()));
        painter.drawPie(rect, static_cast<int>(startAngle), static_cast<int>(spanAngle));
        startAngle += spanAngle;
    }
    
    // Нет (красный)
    if (noPercent > 0) {
        double spanAngle = (noPercent / total) * 360 * 16;
        painter.setBrush(QBrush(ColorPalette::deleteButtonColor()));
        painter.drawPie(rect, static_cast<int>(startAngle), static_cast<int>(spanAngle));
        startAngle += spanAngle;
    }
    
    // Не могу ответить (синий)
    if (unsurePercent > 0) {
        double spanAngle = (unsurePercent / total) * 360 * 16;
        painter.setBrush(QBrush(ColorPalette::actionButtonColor()));
        painter.drawPie(rect, static_cast<int>(startAngle), static_cast<int>(spanAngle));
    }
    
    // Рисуем легенду
    drawLegend(painter, chartX, chartY + chartWidth + 15, chartWidth);
}

void ChartWidget::drawLegend(QPainter& painter, int x, int y, int chartWidth) {
    painter.setPen(Qt::black);
    QFont font("Arial", 11);
    font.setBold(false);
    painter.setFont(font);
    
    int legendItemHeight = 25;
    int legendItemSpacing = 5;
    int colorBoxSize = 18;
    int textOffset = colorBoxSize + 8;
    
    // Центрируем легенду под диаграммой
    // Ширина легенды должна быть достаточной для текста
    int legendWidth = qMin(chartWidth, 250);  // Максимальная ширина легенды
    int legendStartX = x + (chartWidth - legendWidth) / 2;
    
    int currentY = y;
    
    // Всегда показываем все три элемента легенды с процентами
    QList<QPair<QString, QColor>> legendItems;
    legendItems.append(QPair<QString, QColor>("Да", ColorPalette::addButtonColor()));
    legendItems.append(QPair<QString, QColor>("Нет", ColorPalette::deleteButtonColor()));
    legendItems.append(QPair<QString, QColor>("Не могу ответить", ColorPalette::actionButtonColor()));
    
    QList<double> percents;
    percents.append(yesPercent);
    percents.append(noPercent);
    percents.append(unsurePercent);
    
    // Рисуем рамку для легенды
    int legendBoxHeight = legendItems.size() * (legendItemHeight + legendItemSpacing) - legendItemSpacing + 10;
    int legendBoxWidth = legendWidth;
    painter.setPen(QPen(Qt::gray, 1));
    painter.setBrush(QBrush(Qt::white));
    painter.drawRoundedRect(legendStartX - 5, currentY - 5, legendBoxWidth + 10, legendBoxHeight + 10, 5, 5);
    
    // Рисуем элементы легенды
    painter.setPen(Qt::black);
    for (int i = 0; i < legendItems.size(); ++i) {
        int itemX = legendStartX;
        int itemY = currentY;
        
        // Цветной квадрат
        painter.setBrush(QBrush(legendItems[i].second));
        painter.setPen(QPen(Qt::black, 1));
        painter.drawRect(itemX, itemY, colorBoxSize, colorBoxSize);
        
        // Текст с процентом
        QString label = legendItems[i].first;
        QString percentText = (percents[i] > 0) ? 
            QString("%1: %2%").arg(label).arg(QString::number(percents[i], 'f', 1)) :
            QString("%1: 0%").arg(label);
        
        painter.drawText(itemX + textOffset, itemY + colorBoxSize - 3, percentText);
        
        currentY += legendItemHeight + legendItemSpacing;
    }
}

StatisticsWindow::StatisticsWindow(const Test& test, QWidget *parent)
    : QDialog(parent), test(test) {
    setWindowTitle("Статистика теста: " + test.getName());
    setModal(true);
    setMinimumSize(800, 600);
    setupUI();
    updateStatistics();
}

StatisticsWindow::~StatisticsWindow() {}

void StatisticsWindow::setupUI() {
    setStyleSheet(QString("background-color: %1;").arg(ColorPalette::backgroundColor().name()));
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel* titleLabel = new QLabel("Статистика по тесту", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Таблица статистики
    statsTable = new QTableWidget(this);
    statsTable->setColumnCount(5);
    QStringList headers;
    headers << "Вопрос" << "Всего ответов" << "Да" << "Нет" << "Не могу ответить";
    statsTable->setHorizontalHeaderLabels(headers);
    statsTable->horizontalHeader()->setStretchLastSection(true);
    statsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    statsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statsTable->verticalHeader()->setVisible(false);
    statsTable->setStyleSheet(
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
    mainLayout->addWidget(statsTable);
    
    // Диаграмма
    QLabel* chartLabel = new QLabel("Общая статистика по всем вопросам:", this);
    chartLabel->setStyleSheet("font-size: 14px; font-weight: bold; margin: 10px 0;");
    mainLayout->addWidget(chartLabel);
    
    chartWidget = new ChartWidget(this);
    chartWidget->setStyleSheet("background-color: white; border: 1px solid #ddd; border-radius: 5px;");
    mainLayout->addWidget(chartWidget);
    
    // Информация о автоматическом сохранении статистики
    QLabel* saveInfoLabel = new QLabel("💾 Статистика автоматически обновляется и сохраняется при каждом прохождении теста", this);
    saveInfoLabel->setAlignment(Qt::AlignCenter);
    saveInfoLabel->setWordWrap(true);
    saveInfoLabel->setStyleSheet("font-size: 12px; color: #666; margin: 10px;");
    mainLayout->addWidget(saveInfoLabel);
}

void StatisticsWindow::updateStatistics() {
    statistics.collectStatistics(test);
    
    const auto& allStats = statistics.getAllStats();
    statsTable->setRowCount(static_cast<int>(allStats.size()));
    
    double totalYes = 0, totalNo = 0, totalUnsure = 0;
    int totalAnswers = 0;
    
    int row = 0;
    for (const auto& pair : allStats) {
        const auto& stats = pair.second;
        
        statsTable->setItem(row, 0, new QTableWidgetItem(QString("Вопрос %1").arg(pair.first + 1)));
        statsTable->setItem(row, 1, new QTableWidgetItem(QString::number(stats.totalAnswers)));
        statsTable->setItem(row, 2, new QTableWidgetItem(QString("%1 (%2%)")
            .arg(stats.yesCount).arg(QString::number(stats.yesPercent, 'f', 1))));
        statsTable->setItem(row, 3, new QTableWidgetItem(QString("%1 (%2%)")
            .arg(stats.noCount).arg(QString::number(stats.noPercent, 'f', 1))));
        statsTable->setItem(row, 4, new QTableWidgetItem(QString("%1 (%2%)")
            .arg(stats.unsureCount).arg(QString::number(stats.unsurePercent, 'f', 1))));
        
        totalYes += stats.yesCount;
        totalNo += stats.noCount;
        totalUnsure += stats.unsureCount;
        totalAnswers += stats.totalAnswers;
        
        row++;
    }
    
    statsTable->resizeColumnsToContents();
    
    // Обновить диаграмму
    if (totalAnswers > 0) {
        double yesPercent = (totalYes * 100.0) / totalAnswers;
        double noPercent = (totalNo * 100.0) / totalAnswers;
        double unsurePercent = (totalUnsure * 100.0) / totalAnswers;
        chartWidget->setData(yesPercent, noPercent, unsurePercent);
    }
}

// Функция onSaveStatisticsClicked больше не нужна, так как статистика сохраняется автоматически
// Статистика автоматически обновляется и сохраняется при каждом прохождении теста

