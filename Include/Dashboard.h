#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QFrame>
#include "Training.h"
#include "DataManager.h"

class Dashboard : public QWidget {
    Q_OBJECT
private:
    QStackedWidget* m_contentStack;
    QString m_currentUser;

    QLabel* bestWeightLabel;
    QLabel* bestBMILabel;
    QProgressBar* dietBar;      // Pasek diety
    QProgressBar* trainingBar;  // Pasek treningu

public:
    explicit Dashboard(const QString& username, QWidget* parent = nullptr)
        : QWidget(parent), m_currentUser(username) {

        auto mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        // --- SIDEBAR (Bez zmian) ---
        auto sidebar = new QFrame();
        sidebar->setObjectName("SideMenu");
        sidebar->setFixedWidth(240);
        auto sideLayout = new QVBoxLayout(sidebar);

        QStringList menu = { "Pulpit", "Trening", "Dieta", "BMI", "Mentor AI" };
        for (int i = 0; i < menu.size(); ++i) {
            auto btn = new QPushButton(menu[i]);
            btn->setObjectName("NavButton");
            btn->setCheckable(true);
            if (i == 0) btn->setChecked(true);
            sideLayout->addWidget(btn);

            connect(btn, &QPushButton::clicked, [this, i, sideLayout]() {
                m_contentStack->setCurrentIndex(i);
                if (i == 0) refreshData();
                for (int j = 0; j < sideLayout->count(); j++) {
                    auto w = sideLayout->itemAt(j)->widget();
                    if (auto b = qobject_cast<QPushButton*>(w)) b->setChecked(false);
                }
                if (auto b = qobject_cast<QPushButton*>(sender())) b->setChecked(true);
                });
        }
        sideLayout->addStretch();

        // --- CONTENT ---
        m_contentStack = new QStackedWidget();
        m_contentStack->addWidget(createMainDashboard());
        m_contentStack->addWidget(createTrainingPanel());
        m_contentStack->addWidget(createDietPanel());
        m_contentStack->addWidget(new QWidget());
        m_contentStack->addWidget(createMentorPanel());

        mainLayout->addWidget(sidebar);
        mainLayout->addWidget(m_contentStack);

        refreshData();
    }

    QWidget* createMainDashboard() {
        auto page = new QWidget();
        auto layout = new QVBoxLayout(page);
        layout->setContentsMargins(30, 30, 30, 30);
        layout->setSpacing(20);

        // 1. Najlepsze Wyniki
        auto topSection = new QHBoxLayout();
        bestWeightLabel = new QLabel("-- kg");
        bestBMILabel = new QLabel("--");
        topSection->addWidget(createStatCard("Najlepsza Waga", bestWeightLabel, "#6200EE"));
        topSection->addWidget(createStatCard("Najlepsze BMI", bestBMILabel, "#03DAC6"));
        layout->addLayout(topSection);

        // 2. Dieta (Z tekstami w środku paska!)
        auto dietFrame = new QFrame();
        dietFrame->setProperty("class", "Card");
        auto dietLayout = new QVBoxLayout(dietFrame);

        auto dietHeader = new QLabel("Dzisiejsza Dieta");
        dietHeader->setProperty("class", "Header");

        dietBar = new QProgressBar();
        dietBar->setStyleSheet("QProgressBar { border: 1px solid #ddd; border-radius: 5px; text-align: center; height: 30px; color: white; font-weight: bold; } QProgressBar::chunk { background-color: #6200EE; border-radius: 5px;}");

        // --- ZMIANA: Włączamy tekst w pasku i ustawiamy format "Wartość / Max" ---
        dietBar->setTextVisible(true);
        dietBar->setFormat("%v / %m kcal");

        dietLayout->addWidget(dietHeader);
        dietLayout->addWidget(dietBar);
        layout->addWidget(dietFrame);

        // 3. Trening (Z tekstami w środku paska!)
        auto trainingFrame = new QFrame();
        trainingFrame->setProperty("class", "Card");
        auto trainingLayout = new QVBoxLayout(trainingFrame);

        auto trainingHeader = new QLabel("Treningi w tym tygodniu");
        trainingHeader->setProperty("class", "Header");

        trainingBar = new QProgressBar();
        trainingBar->setStyleSheet("QProgressBar { border: 1px solid #ddd; border-radius: 5px; text-align: center; height: 30px; color: white; font-weight: bold; } QProgressBar::chunk { background-color: #03DAC6; border-radius: 5px;}");

        // --- ZMIANA: Format "Wartość / Max" ---
        trainingBar->setTextVisible(true);
        trainingBar->setFormat("%v / %m");

        trainingLayout->addWidget(trainingHeader);
        trainingLayout->addWidget(trainingBar);
        layout->addWidget(trainingFrame);

        // 4. Mentor Button
        auto mentorBtn = new QPushButton("💬 Potrzebujesz motywacji? Napisz do Mentora!");
        mentorBtn->setStyleSheet("background-color: #000000; color: white; font-size: 16px; padding: 15px; border-radius: 8px; font-weight: bold;");
        mentorBtn->setCursor(Qt::PointingHandCursor);
        connect(mentorBtn, &QPushButton::clicked, [this]() { m_contentStack->setCurrentIndex(4); });

        layout->addStretch();
        layout->addWidget(mentorBtn);

        return page;
    }

    QFrame* createStatCard(const QString& title, QLabel* valueLabel, const QString& color) {
        auto card = new QFrame();
        card->setStyleSheet(QString("background-color: %1; border-radius: 10px; color: white;").arg(color));
        card->setMinimumHeight(100);
        auto layout = new QVBoxLayout(card);
        auto titleLbl = new QLabel(title);
        titleLbl->setStyleSheet("font-size: 14px; opacity: 0.8; color: white;");
        valueLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: white;");
        valueLabel->setAlignment(Qt::AlignRight);
        layout->addWidget(titleLbl);
        layout->addWidget(valueLabel);
        return card;
    }

    void refreshData() {
        double bWeight, bBMI;
        DataManager::getBestStats(m_currentUser, bWeight, bBMI);
        bestWeightLabel->setText(bWeight > 0 ? QString::number(bWeight, 'f', 1) + " kg" : "-- kg");
        bestBMILabel->setText(bBMI > 0 ? QString::number(bBMI, 'f', 2) : "--");

        // Dieta
        int goal = DataManager::loadCalorieGoal(m_currentUser);
        int consumed = DataManager::loadConsumedCalories(m_currentUser);
        dietBar->setRange(0, goal > 0 ? goal : 1);
        dietBar->setValue(consumed);
        // Format "%v / %m kcal" sam podstawi liczby w miejsce %v i %m

        // Trening
        int trainings = DataManager::loadWeeklyTrainings(m_currentUser);
        int trainingGoal = DataManager::loadTrainingGoal(m_currentUser);
        if (trainingGoal == 0) trainingGoal = 1;

        trainingBar->setRange(0, trainingGoal);
        trainingBar->setValue(trainings);
        // Format "%v / %m" sam podstawi liczby
    }

    QWidget* createDietPanel() { return new QWidget(); }
    QWidget* createTrainingPanel() { return new QWidget(); }
    QWidget* createMentorPanel() { return new QWidget(); }
    QStackedWidget* getStack() { return m_contentStack; }
};