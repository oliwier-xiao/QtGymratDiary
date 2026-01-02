#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include "Training.h"

class Dashboard : public QWidget {
    Q_OBJECT
private:
    QStackedWidget* m_contentStack;

public:
    explicit Dashboard(QWidget* parent = nullptr) : QWidget(parent) {
        // Konstruktor (pozostawiam bez zmian logikę UI)
        auto mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        auto sidebar = new QFrame();
        sidebar->setObjectName("SideMenu");
        sidebar->setFixedWidth(240);
        auto sideLayout = new QVBoxLayout(sidebar);

        QStringList menu = { "Dashboard", "Trening", "Dieta", "BMI", "Mentor AI" };
        for (int i = 0; i < menu.size(); ++i) {
            auto btn = new QPushButton(menu[i]);
            btn->setObjectName("NavButton");
            sideLayout->addWidget(btn);
        }
        sideLayout->addStretch();

        m_contentStack = new QStackedWidget();
        // Tutaj normalnie dodawalibyśmy widoki...

        mainLayout->addWidget(sidebar);
        mainLayout->addWidget(m_contentStack);
    }

    // --- TE METODY MUSZĄ BYĆ PUBLICZNE I ZDEFINIOWANE RAZ ---

    QWidget* createDietPanel() {
        auto panel = new QWidget();
        auto layout = new QGridLayout(panel);

        QStringList meals = { "Śniadanie", "Lunch", "Obiad", "Kolacja" };
        for (int i = 0; i < meals.size(); ++i) {
            auto card = new QFrame();
            card->setProperty("class", "Card");
            auto cardLayout = new QVBoxLayout(card);

            auto title = new QLabel(meals[i]);
            title->setProperty("class", "Header");

            auto list = new QListWidget();
            auto addBtn = new QPushButton("+ Dodaj produkt");
            addBtn->setProperty("class", "Secondary");

            cardLayout->addWidget(title);
            cardLayout->addWidget(list);
            cardLayout->addWidget(addBtn);

            layout->addWidget(card, i / 2, i % 2);
        }
        return panel;
    }

    QWidget* createTrainingPanel() {
        auto panel = new QWidget();
        auto layout = new QVBoxLayout(panel);
        layout->addWidget(new QLabel("Tutaj będzie lista treningów (TODO)"));
        return panel;
    }

    QWidget* createMentorPanel() {
        auto panel = new QWidget();
        auto layout = new QVBoxLayout(panel);
        layout->addWidget(new QLabel("Tutaj będzie czat z AI (TODO)"));
        return panel;
    }
};