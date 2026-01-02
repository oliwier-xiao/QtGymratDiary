#pragma once 
#include <QMainWindow>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

// Do³¹czamy nasze modu³y
#include "Login.h"        
#include "Dashboard.h"    
#include "DataManager.h"  
#include "BMIWidget.h"    

class MainWindow : public QMainWindow {
    Q_OBJECT

        QStackedWidget* mainStack;
    QWidget* dashboardContainer;
    User* currentUser = nullptr;

public:
    MainWindow() {
        resize(1200, 800);
        DataManager::init();

        mainStack = new QStackedWidget(this);
        setCentralWidget(mainStack);

        // Ekran logowania
        auto loginScreen = new Login();
        mainStack->addWidget(loginScreen);

        // Kontener na Dashboard (pusty, wype³nimy go po zalogowaniu)
        dashboardContainer = new QWidget();
        mainStack->addWidget(dashboardContainer);

        connect(loginScreen, &Login::loginSuccess, this, &MainWindow::onLogin);
    }

    // --- TO JEST TA ZMIENIONA FUNKCJA ---
    void createDashboardUI() {
        // Usuwamy stary layout jeœli istnieje (czyszczenie przy przelogowaniu)
        if (dashboardContainer->layout()) delete dashboardContainer->layout();

        auto mainLayout = new QVBoxLayout(dashboardContainer);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        // Tworzymy Dashboard, przekazuj¹c nazwê u¿ytkownika (¿eby wiedzia³ czyje dane wczytaæ)
        auto dashboard = new Dashboard(currentUser->getLogin());

        // --- TRIK Z BMI ---
        // Dashboard ma w sobie placeholder (puste miejsce) na BMI pod indeksem 3.
        // Podmieniamy je tutaj na prawdziwy BMIWidget, któremu przekazujemy login.
        auto stack = dashboard->getStack();
        if (stack->count() > 3) {
            auto oldWidget = stack->widget(3);      // Bierzemy placeholder
            stack->removeWidget(oldWidget);         // Wywalamy go
            stack->insertWidget(3, new BMIWidget(currentUser->getLogin())); // Wstawiamy BMI
            delete oldWidget;                       // Czyœcimy pamiêæ po placeholderze
        }

        mainLayout->addWidget(dashboard);
    }

    void onLogin(User* user) {
        currentUser = user;
        createDashboardUI(); // Budujemy interfejs dopiero teraz, gdy mamy u¿ytkownika
        mainStack->setCurrentWidget(dashboardContainer);
        setWindowTitle("Gymrat Diary - Zalogowany jako: " + user->getName());
    }
};