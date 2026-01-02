#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>
#include "Logic/Health.h"
#include "Core/DataManager.h"

class BMIWidget : public QWidget {
    Q_OBJECT
        QLineEdit* weightInput;
    QLineEdit* heightInput;
    QComboBox* goalSelect;
    QComboBox* activitySelect;
    QLabel* resultLabel;
    QString m_currentUser;

public:
    BMIWidget(const QString& username, QWidget* parent = nullptr)
        : QWidget(parent), m_currentUser(username) {

        auto layout = new QVBoxLayout(this);
        auto card = new QFrame();
        card->setObjectName("Card"); // Ważne dla stylu tła
        auto cardLayout = new QVBoxLayout(card);

        auto header = new QLabel("Kalkulator BMI & Zapotrzebowania");
        header->setProperty("class", "Header");

        heightInput = new QLineEdit(); heightInput->setPlaceholderText("Wzrost (cm)");
        weightInput = new QLineEdit(); weightInput->setPlaceholderText("Waga (kg)");

        auto goalLabel = new QLabel("Twój cel:");
        goalSelect = new QComboBox();
        goalSelect->addItem("Utrzymanie wagi (Zero kaloryczne)", 0);
        goalSelect->addItem("Redukcja (-300 kcal)", -300);
        goalSelect->addItem("Masa (+300 kcal)", 300);

        auto activityLabel = new QLabel("Aktywność w tygodniu:");
        activitySelect = new QComboBox();
        activitySelect->addItem("Brak treningów (Siedzący)", 1.2);
        activitySelect->addItem("1-2 treningi (Lekki)", 1.35);
        activitySelect->addItem("3-4 treningi (Umiarkowany)", 1.55);
        activitySelect->addItem("5+ treningów (Duży)", 1.75);

        auto calcBtn = new QPushButton("Oblicz i Zapisz Profil");
        // --- ZMIANA: Używamy PrimaryButton (Fioletowy) ---
        calcBtn->setObjectName("PrimaryButton");
        calcBtn->setCursor(Qt::PointingHandCursor);
        calcBtn->setMinimumHeight(45);

        resultLabel = new QLabel("Wynik: --");
        resultLabel->setStyleSheet("font-size: 16px; color: #3700B3; margin-top: 15px;");
        resultLabel->setWordWrap(true);

        cardLayout->addWidget(header);
        cardLayout->addWidget(heightInput);
        cardLayout->addWidget(weightInput);
        cardLayout->addSpacing(10);
        cardLayout->addWidget(goalLabel);
        cardLayout->addWidget(goalSelect);
        cardLayout->addWidget(activityLabel);
        cardLayout->addWidget(activitySelect);
        cardLayout->addSpacing(15);
        cardLayout->addWidget(calcBtn);
        cardLayout->addWidget(resultLabel);

        layout->addWidget(card);
        layout->addStretch();

        connect(calcBtn, &QPushButton::clicked, [this]() {
            bool okW, okH;
            double w = weightInput->text().toDouble(&okW);
            double h = heightInput->text().toDouble(&okH);

            if (okW && okH && h > 0) {
                BMIRecord record(w, h);
                double bmr = (10 * w) + (6.25 * h) - (5 * 25) + 5;
                double activityMultiplier = activitySelect->currentData().toDouble();
                double tdee = bmr * activityMultiplier;
                int finalCalories = static_cast<int>(tdee + goalSelect->currentData().toInt());

                int activityIndex = activitySelect->currentIndex();
                int trainingGoal = 0;
                if (activityIndex == 1) trainingGoal = 2;
                else if (activityIndex == 2) trainingGoal = 4;
                else if (activityIndex == 3) trainingGoal = 7;

                QString goalName = goalSelect->currentText().split("(")[0];

                resultLabel->setText(
                    "BMI: " + QString::number(record.calculate(), 'f', 2) + " (" + record.getCategory() + ")\n" +
                    "Zapotrzebowanie: " + QString::number(finalCalories) + " kcal\n" +
                    "Cel treningowy: " + QString::number(trainingGoal) + " / tydz."
                );
                DataManager::saveFullProfile(m_currentUser, record, finalCalories, trainingGoal);
            }
            else {
                resultLabel->setText("Błąd: Podaj poprawną wagę i wzrost!");
            }
            });
    }
};