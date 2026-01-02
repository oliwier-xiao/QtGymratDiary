#pragma once 
#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>

#include "Dashboard.h"    
#include "BMIWidget.h"
#include "../Core/User.h"
#include "../Core/DataManager.h"  

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QStackedWidget* mainStack;
    QWidget* dashboardContainer;
    User* currentUser = nullptr;

public:
    // Konstruktor WYMAGA u¿ytkownika
    explicit MainWindow(User* user, QWidget* parent = nullptr)
        : QMainWindow(parent), currentUser(user)
    {
        resize(1200, 800);
        DataManager::init();

        if (currentUser) {
            setWindowTitle("Gymrat Diary - Zalogowany jako: " + currentUser->getName());
        }

        mainStack = new QStackedWidget(this);
        setCentralWidget(mainStack);

        dashboardContainer = new QWidget();
        mainStack->addWidget(dashboardContainer);

        createDashboardUI();
    }

    void createDashboardUI() {
        if (!currentUser) return;

        // Czyœcimy stary widok
        if (dashboardContainer->layout()) {
            QLayout* oldLayout = dashboardContainer->layout();
            QLayoutItem* item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }

        auto mainLayout = new QVBoxLayout(dashboardContainer);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        auto dashboard = new Dashboard(currentUser->getLogin(), currentUser->getName());

        auto stack = dashboard->getStack();
        if (stack && stack->count() > 3) {
            auto oldWidget = stack->widget(3);
            stack->removeWidget(oldWidget);
            stack->insertWidget(3, new BMIWidget(currentUser->getLogin()));
            delete oldWidget;
        }

        mainLayout->addWidget(dashboard);
    }
};