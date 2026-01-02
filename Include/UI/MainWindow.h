#pragma once 
#include <QMainWindow>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "Login.h"        
#include "Dashboard.h"    
#include "Core/DataManager.h"  
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

        auto loginScreen = new Login();
        mainStack->addWidget(loginScreen);

        dashboardContainer = new QWidget();
        mainStack->addWidget(dashboardContainer);

        connect(loginScreen, &Login::loginSuccess, this, &MainWindow::onLogin);
    }

    void createDashboardUI() {
        if (dashboardContainer->layout()) delete dashboardContainer->layout();
        auto mainLayout = new QVBoxLayout(dashboardContainer);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        // --- ZMIANA: Przekazujemy login ORAZ imiê ---
        auto dashboard = new Dashboard(currentUser->getLogin(), currentUser->getName());

        auto stack = dashboard->getStack();
        if (stack->count() > 3) {
            auto oldWidget = stack->widget(3);
            stack->removeWidget(oldWidget);
            stack->insertWidget(3, new BMIWidget(currentUser->getLogin()));
            delete oldWidget;
        }

        mainLayout->addWidget(dashboard);
    }

    void onLogin(User* user) {
        currentUser = user;
        createDashboardUI();
        mainStack->setCurrentWidget(dashboardContainer);
        setWindowTitle("Gymrat Diary - Zalogowany jako: " + user->getName());
    }
};