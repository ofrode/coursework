#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QColor>
#include <QString>

// Класс для работы с цветовой палитрой
class ColorPalette {
public:
    // Цвета из палитры (9 цветов)
    static QColor lightTeal() { return QColor(173, 216, 200); }      // Светло-бирюзовый
    static QColor lightYellow() { return QColor(255, 248, 200); }   // Светло-желтый
    static QColor lightPink() { return QColor(255, 200, 200); }     // Светло-розовый
    static QColor paleBlue() { return QColor(200, 230, 255); }      // Бледно-голубой
    static QColor lightPeach() { return QColor(255, 230, 200); }    // Светло-персиковый
    static QColor dustyRose() { return QColor(240, 180, 180); }     // Пыльно-розовый
    static QColor lightOrange() { return QColor(255, 210, 180); }   // Светло-оранжевый
    static QColor paleYellow() { return QColor(255, 250, 220); }    // Бледно-желтый
    static QColor veryLightPink() { return QColor(255, 220, 220); } // Очень светло-розовый
    
    // Цвета для кнопок
    static QColor addButtonColor() { return lightTeal(); }
    static QColor deleteButtonColor() { return dustyRose(); }
    static QColor actionButtonColor() { return paleBlue(); }
    
    // Фоновые цвета
    static QColor backgroundColor() { return QColor(250, 250, 250); }
    static QColor windowBackground() { return QColor(255, 255, 255); }
};

#endif // COLORPALETTE_H

