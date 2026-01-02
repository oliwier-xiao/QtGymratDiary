#include <QApplication>
#include <QIcon>
#include "UI/Login.h"        // Startujemy od Logowania
#include "Core/Theme.h"
#include "Core/DataManager.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

   
    DataManager::init();

   
    a.setStyleSheet(Theme::APP_STYLE);


    Login w;
    w.show();

    return a.exec();
}