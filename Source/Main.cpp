#include <QApplication>
#include "UI/MainWindow.h"
#include "UI/Login.h"
#include "Core/Theme.h"
#include "Core/DataManager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    // Aplikacja globalnego stylu
    a.setStyleSheet(Theme::APP_STYLE);

    MainWindow w;
    w.show();

    return a.exec();
}