#include <QApplication>
#include "ui/MainWindow.h"
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    app.setStyle(QStyleFactory::create("Fusion"));
    
    MainWindow window;
    window.show();
    
    return app.exec();
}