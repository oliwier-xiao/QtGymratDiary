#include <QApplication>
#include "MainWindow.h"
#include "Theme.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    // Aplikacja globalnego stylu
    a.setStyleSheet(Theme::APP_STYLE);

    MainWindow w;
    w.show();

    return a.exec();
}