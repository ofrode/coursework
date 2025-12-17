#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QColor>
#include <QString>

class ColorPalette {
public:
    static QColor lightTeal() { return QColor(173, 216, 200); }
    static QColor lightYellow() { return QColor(255, 248, 200); }
    static QColor lightPink() { return QColor(255, 200, 200); }
    static QColor paleBlue() { return QColor(200, 230, 255); }
    static QColor lightPeach() { return QColor(255, 230, 200); }
    static QColor dustyRose() { return QColor(240, 180, 180); }
    static QColor lightOrange() { return QColor(255, 210, 180); }
    static QColor paleYellow() { return QColor(255, 250, 220); }
    static QColor veryLightPink() { return QColor(255, 220, 220); }
    
    static QColor addButtonColor() { return lightTeal(); }
    static QColor deleteButtonColor() { return dustyRose(); }
    static QColor actionButtonColor() { return paleBlue(); }
    
    static QColor backgroundColor() { return QColor(250, 250, 250); }
    static QColor windowBackground() { return QColor(255, 255, 255); }
};

#endif

