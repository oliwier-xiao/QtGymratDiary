#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include "Health.h"
#include "DataManager.h"

class BMIWidget : public QWidget {
    Q_OBJECT
        QLineEdit* weightInput;
    QLineEdit* heightInput;
    QLabel* resultLabel;
    QString m_currentUser;

public:
    BMIWidget(const QString& username, QWidget* parent = nullptr)
        : QWidget(parent), m_currentUser(username) {

        auto layout = new QVBoxLayout(this);

        auto card = new QFrame();
        card->setProperty("class", "Card");
        auto cardLayout = new QVBoxLayout(card);

        auto header = new QLabel("Kalkulator BMI");
        header->setProperty("class", "Header");

        // --- ZMIANA KOLEJNOŚCI (1/2): Najpierw tworzymy Wzrost (dla TAB order) ---
        heightInput = new QLineEdit();
        heightInput->setPlaceholderText("Wzrost (cm)");

        weightInput = new QLineEdit();
        weightInput->setPlaceholderText("Waga (kg)");

        auto calcBtn = new QPushButton("Oblicz i Zapisz");
        calcBtn->setProperty("class", "Secondary");

        resultLabel = new QLabel("Wynik: --");
        resultLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #3700B3; margin-top: 10px;");

        // --- ZMIANA KOLEJNOŚCI (2/2): Najpierw dodajemy do widoku Wzrost ---
        cardLayout->addWidget(header);
        cardLayout->addWidget(heightInput); 
        cardLayout->addWidget(weightInput); 
        cardLayout->addWidget(calcBtn);
        cardLayout->addWidget(resultLabel);

        layout->addWidget(card);
        layout->addStretch();

        connect(calcBtn, &QPushButton::clicked, [this]() {
            bool okW, okH;
            double w = weightInput->text().toDouble(&okW);
            double h = heightInput->text().toDouble(&okH);

            if (okW && okH && h > 0) {
                // Logika pozostaje bez zmian: pierwszy argument to waga, drugi wzrost
                BMIRecord record(w, h);

                resultLabel->setText("BMI: " + QString::number(record.calculate(), 'f', 2) +
                    "\n" + record.getCategory());

                DataManager::saveBMILog(m_currentUser, record);
            }
            else {
                resultLabel->setText("Błąd: Podaj poprawne liczby!");
            }
            });
    }
};