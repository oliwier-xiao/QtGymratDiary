#pragma once // Wa¿ne!
#include <QMainWindow>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

// POPRAWKA 1: Do³¹czamy w³aœciwe nag³ówki
#include "Login.h"        // Zamiast LoginWidget.h
#include "Dashboard.h"    // Zamiast DashboardWidget.h
#include "DataManager.h"  // POPRAWKA 2: Brakowa³o tego do DataManager::init()
#include "BMIWidget.h"       // Do obs³ugi BMI

class MainWindow : public QMainWindow {
    Q_OBJECT // Wa¿ne dla Qt!

        QStackedWidget* mainStack;
    QWidget* dashboardContainer;
    QStackedWidget* contentStack;
    User* currentUser = nullptr;

public:
    MainWindow() {
        resize(1200, 800);
        DataManager::init(); // Teraz zadzia³a, bo dodaliœmy include

        mainStack = new QStackedWidget(this);
        setCentralWidget(mainStack);

        // POPRAWKA 3: U¿ywamy klasy Login (zgodnie z plikiem Login.h)
        auto loginScreen = new Login();
        mainStack->addWidget(loginScreen);

        // Dashboard budujemy dopiero po zalogowaniu, ale kontener przygotowujemy
        dashboardContainer = new QWidget(); // Placeholder
        mainStack->addWidget(dashboardContainer);

        connect(loginScreen, &Login::loginSuccess, this, &MainWindow::onLogin);
    }

    void createDashboardUI() {
        // Usuwamy stary layout jeœli istnieje (proste czyszczenie)
        if (dashboardContainer->layout()) delete dashboardContainer->layout();

        auto mainLayout = new QHBoxLayout(dashboardContainer);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        // --- Sidebar ---
        auto sideMenu = new QFrame();
        sideMenu->setObjectName("SideMenu");
        sideMenu->setFixedWidth(250);
        auto menuLayout = new QVBoxLayout(sideMenu);
        menuLayout->setContentsMargins(0, 20, 0, 20);

        auto logo = new QLabel("GYMRAT\nDIARY");
        logo->setAlignment(Qt::AlignCenter);
        logo->setStyleSheet("color: white; font-weight: bold; font-size: 24px; margin-bottom: 30px;");
        menuLayout->addWidget(logo);

        auto btnGroup = new QButtonGroup(this);
        QStringList menuItems = { "Pulpit", "Trening", "Dieta", "BMI", "Mentor AI" };

        contentStack = new QStackedWidget();

        // --- Tworzenie przycisków menu ---
        for (int i = 0; i < menuItems.size(); ++i) {
            auto btn = new QPushButton(menuItems[i]);
            btn->setCheckable(true);
            btn->setObjectName("NavButton");
            if (i == 0) btn->setChecked(true);

            menuLayout->addWidget(btn);
            btnGroup->addButton(btn, i);
            connect(btn, &QPushButton::clicked, [=]() { contentStack->setCurrentIndex(i); });
        }
        menuLayout->addStretch();

        // --- Content Area (Tworzymy widoki) ---
        // Tutaj tworzymy instancjê Dashboard (z pliku Dashboard.h) dla widoku "Dieta/Trening"
        auto dashboardView = new Dashboard();

        contentStack->addWidget(new QLabel("Witaj w panelu g³ównym!")); // 0: Pulpit
        contentStack->addWidget(dashboardView->createTrainingPanel()); // 1: Trening (metoda z Dashboard)
        contentStack->addWidget(dashboardView->createDietPanel());     // 2: Dieta (metoda z Dashboard)
        contentStack->addWidget(new BMIWidget(currentUser->getLogin()));                     // 3: BMI 
        contentStack->addWidget(new QLabel("Czat z Mentorem"));        // 4: Mentor

        mainLayout->addWidget(sideMenu);
        mainLayout->addWidget(contentStack);
    }

    void onLogin(User* user) {
        currentUser = user;
        createDashboardUI(); // Budujemy interfejs dopiero teraz
        mainStack->setCurrentWidget(dashboardContainer);
        setWindowTitle("Gymrat Diary - Zalogowany jako: " + user->getName());
    }
};