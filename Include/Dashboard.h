#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QFrame>
#include <QListWidget> 
#include <QInputDialog>
#include <QMessageBox> 
#include <QLineEdit>       
#include <QScrollArea>     
#include <QTimer>          
#include <QScrollBar>      

#include "StatsManager.h"
#include "DiarySystem.h"
#include "Mentor.h"   

class Dashboard : public QWidget {
    Q_OBJECT
private:
    QStackedWidget* m_contentStack;
    QString m_currentUser;
    QString m_displayName;

    StatsManager m_statsManager;

    // UI Elements
    QLabel* bestWeightLabel;
    QLabel* bestBMILabel;
    QProgressBar* dietBar;
    QProgressBar* trainingBar;
    QListWidget* dietListWidget;
    QListWidget* trainingListWidget;

    // --- ELEMENTY CZATU ---
    QWidget* chatContainer;
    QVBoxLayout* chatLayout;
    QLineEdit* chatInput;
    QScrollArea* chatScroll;

public:
    explicit Dashboard(const QString& username, const QString& displayName, QWidget* parent = nullptr)
        : QWidget(parent), m_currentUser(username), m_displayName(displayName) {

        auto mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

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
                if (i == 0 || i == 1 || i == 2) refreshData();
                for (int j = 0; j < sideLayout->count(); j++) {
                    auto w = sideLayout->itemAt(j)->widget();
                    if (auto b = qobject_cast<QPushButton*>(w)) b->setChecked(false);
                }
                if (auto b = qobject_cast<QPushButton*>(sender())) b->setChecked(true);
                });
        }
        sideLayout->addStretch();

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

    // --- FUNKCJE CZATU ---

    void addChatMessage(const QString& text, bool isUser) {
        auto rowWidget = new QWidget();
        auto rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(0, 5, 0, 5);

        auto bubble = new QLabel(text);
        bubble->setWordWrap(true);
        bubble->setTextFormat(Qt::MarkdownText);
        bubble->setFont(QFont("Segoe UI", 14));
        bubble->setContentsMargins(15, 10, 15, 10);
        bubble->setMaximumWidth(600);

        if (isUser) {
            // Styl Użytkownika (Fioletowy)
            bubble->setStyleSheet("background-color: #6200EE; color: white; border-radius: 15px; border-bottom-right-radius: 2px;");
            rowLayout->addStretch();
            rowLayout->addWidget(bubble);
        }
        else {
            // Styl Mentora (Szary)
            bubble->setStyleSheet("background-color: #E0E0E0; color: black; border-radius: 15px; border-bottom-left-radius: 2px;");
            rowLayout->addWidget(bubble);
            rowLayout->addStretch();
        }

        chatLayout->addWidget(rowWidget);

        // Przewiń na dół
        QTimer::singleShot(50, [this]() {
            if (chatScroll && chatScroll->verticalScrollBar())
                chatScroll->verticalScrollBar()->setValue(chatScroll->verticalScrollBar()->maximum());
            });
    }

    void handleSendMessage() {
        QString text = chatInput->text().trimmed();
        if (text.isEmpty()) return;

        // 1. Pokaż co napisał user
        addChatMessage(text, true);
        chatInput->clear();

        // 2. Odpowiedź Mentora (z lekkim opóźnieniem)
        QTimer::singleShot(600, [this, text]() {
            QString response = Mentor::getResponse(text);
            addChatMessage(response, false);
            });
    }

    QWidget* createMentorPanel() {
        auto panel = new QWidget();
        auto layout = new QVBoxLayout(panel);
        layout->setContentsMargins(20, 20, 20, 20);

        auto header = new QLabel("Mentor AI");
        header->setProperty("class", "Header");
        header->setAlignment(Qt::AlignCenter);

        chatScroll = new QScrollArea();
        chatScroll->setWidgetResizable(true);
        chatScroll->setFrameShape(QFrame::NoFrame);
        chatScroll->setStyleSheet("background-color: white; border-radius: 10px; border: 1px solid #E0E0E0;");

        chatContainer = new QWidget();
        chatContainer->setStyleSheet("background-color: transparent;");
        chatLayout = new QVBoxLayout(chatContainer);
        chatLayout->addStretch();
        chatScroll->setWidget(chatContainer);

        auto inputContainer = new QWidget();
        auto inputLayout = new QHBoxLayout(inputContainer);
        inputLayout->setContentsMargins(0, 10, 0, 0);

        chatInput = new QLineEdit();
        chatInput->setPlaceholderText("Napisz np. 'trening klatka', 'co na obiad', 'motywacja'...");
        chatInput->setMinimumHeight(50);
        chatInput->setStyleSheet("border-radius: 25px; padding-left: 20px; font-size: 14px; border: 2px solid #E0E0E0;");

        auto sendBtn = new QPushButton("➤");
        sendBtn->setCursor(Qt::PointingHandCursor);
        sendBtn->setFixedSize(50, 50);
        sendBtn->setStyleSheet("QPushButton { background-color: #6200EE; color: white; border-radius: 25px; font-size: 20px; } QPushButton:hover { background-color: #3700B3; }");

        inputLayout->addWidget(chatInput);
        inputLayout->addWidget(sendBtn);

        layout->addWidget(header);
        layout->addWidget(chatScroll);
        layout->addWidget(inputContainer);

        connect(sendBtn, &QPushButton::clicked, this, &Dashboard::handleSendMessage);
        connect(chatInput, &QLineEdit::returnPressed, this, &Dashboard::handleSendMessage);

        // Powitanie na start
        QTimer::singleShot(500, [this]() {
            if (chatLayout->count() <= 1) {
                addChatMessage("Cześć! Jestem Twoim Mentorem. 💪\nPytaj śmiało o treningi, dietę albo poproś o dawkę motywacji!", false);
            }
            });

        return panel;
    }

    // --- FUNKCJE PULPITU ---

    QWidget* createMainDashboard() {
        auto page = new QWidget();
        auto layout = new QVBoxLayout(page);
        layout->setContentsMargins(30, 30, 30, 30);
        layout->setSpacing(20);

        auto welcomeLabel = new QLabel("Witam, " + m_displayName + "!");
        welcomeLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #333; margin-bottom: 10px;");
        layout->addWidget(welcomeLabel);

        auto topSection = new QHBoxLayout();
        bestWeightLabel = new QLabel("-- kg");
        bestBMILabel = new QLabel("--");
        topSection->addWidget(createStatCard("Najlepsza Waga", bestWeightLabel, "#6200EE"));
        topSection->addWidget(createStatCard("Najlepsze BMI", bestBMILabel, "#03DAC6"));
        layout->addLayout(topSection);

        auto dietFrame = new QFrame(); dietFrame->setObjectName("Card");
        auto dietLayout = new QVBoxLayout(dietFrame);
        auto dietHeader = new QLabel("Dzisiejsza Dieta"); dietHeader->setProperty("class", "Header");
        dietBar = new QProgressBar();
        dietBar->setStyleSheet("QProgressBar { border: 1px solid #ddd; border-radius: 5px; text-align: center; height: 30px; color: white; font-weight: bold; background-color: #333; } QProgressBar::chunk { background-color: #6200EE; border-radius: 5px;}");
        dietBar->setTextVisible(true); dietBar->setFormat("%v / %m kcal");
        dietLayout->addWidget(dietHeader); dietLayout->addWidget(dietBar);
        layout->addWidget(dietFrame);

        auto trainingFrame = new QFrame(); trainingFrame->setObjectName("Card");
        auto trainingLayout = new QVBoxLayout(trainingFrame);
        auto trainingHeader = new QLabel("Treningi w tym tygodniu"); trainingHeader->setProperty("class", "Header");
        trainingBar = new QProgressBar();
        trainingBar->setStyleSheet("QProgressBar { border: 1px solid #ddd; border-radius: 5px; text-align: center; height: 30px; color: white; font-weight: bold; background-color: #333; } QProgressBar::chunk { background-color: #03DAC6; border-radius: 5px;}");
        trainingBar->setTextVisible(true); trainingBar->setFormat("%v / %m");
        trainingLayout->addWidget(trainingHeader); trainingLayout->addWidget(trainingBar);
        layout->addWidget(trainingFrame);

        auto mentorBtn = new QPushButton("💬 Potrzebujesz motywacji? Napisz do Mentora!");
        mentorBtn->setObjectName("PrimaryButton");
        connect(mentorBtn, &QPushButton::clicked, [this]() { m_contentStack->setCurrentIndex(4); });
        layout->addStretch(); layout->addWidget(mentorBtn);
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
        layout->addWidget(titleLbl); layout->addWidget(valueLabel);
        return card;
    }

    QWidget* createListItemWidget(const QString& text, const QString& serializedData) {
        auto widget = new QWidget();
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(5, 5, 5, 5);

        auto label = new QLabel(text);
        label->setStyleSheet("font-size: 14px; color: #333; font-weight: 500;");
        auto delBtn = new QPushButton("X");
        delBtn->setObjectName("DeleteButton");
        delBtn->setCursor(Qt::PointingHandCursor);

        connect(delBtn, &QPushButton::clicked, [this, serializedData]() {
            auto reply = QMessageBox::question(this, "Usuń", "Czy na pewno usunąć ten wpis?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                m_statsManager.removeEntry(m_currentUser, serializedData);
                refreshData();
            }
            });
        layout->addWidget(label);
        layout->addStretch();
        layout->addWidget(delBtn);
        return widget;
    }

    void refreshData() {
        double bWeight, bBMI;
        DataManager::getBestStats(m_currentUser, bWeight, bBMI);
        bestWeightLabel->setText(bWeight > 0 ? QString::number(bWeight, 'f', 1) + " kg" : "-- kg");
        bestBMILabel->setText(bBMI > 0 ? QString::number(bBMI, 'f', 2) : "--");

        DailyStats stats = m_statsManager.getDailyStats(m_currentUser);
        int calorieGoal = m_statsManager.getCalorieGoal(m_currentUser);
        int trainingGoal = m_statsManager.getTrainingGoal(m_currentUser);

        if (dietListWidget) dietListWidget->clear();
        if (trainingListWidget) trainingListWidget->clear();

        for (DiaryEntry* entry : stats.todayEntries) {
            auto item = new QListWidgetItem();
            auto widget = createListItemWidget(entry->getDescription(), entry->serialize());
            item->setSizeHint(widget->sizeHint());

            if (entry->getType() == "MEAL") {
                if (dietListWidget) { dietListWidget->addItem(item); dietListWidget->setItemWidget(item, widget); }
            }
            else if (entry->getType() == "WORKOUT") {
                if (trainingListWidget) { trainingListWidget->addItem(item); trainingListWidget->setItemWidget(item, widget); }
            }
        }
        qDeleteAll(stats.todayEntries);

        dietBar->setRange(0, calorieGoal > 0 ? calorieGoal : 1);
        dietBar->setValue(stats.currentBalance);
        trainingBar->setRange(0, trainingGoal);
        trainingBar->setValue(stats.trainingsCount);
    }

    QWidget* createDietPanel() {
        auto panel = new QWidget();
        auto layout = new QVBoxLayout(panel);
        layout->setContentsMargins(40, 40, 40, 40);
        auto label = new QLabel("Twój Dziennik Posiłków (Dzisiaj)"); label->setProperty("class", "Header");
        dietListWidget = new QListWidget(); dietListWidget->setSelectionMode(QAbstractItemView::NoSelection);
        auto addBtn = new QPushButton("+ Dodaj Posiłek"); addBtn->setCursor(Qt::PointingHandCursor); addBtn->setMinimumHeight(50);
        layout->addWidget(label); layout->addWidget(dietListWidget); layout->addWidget(addBtn);

        connect(addBtn, &QPushButton::clicked, [this]() {
            bool ok;
            QString name = QInputDialog::getText(this, "Nowy Posiłek", "Co zjadłeś?", QLineEdit::Normal, "", &ok);
            if (!ok || name.isEmpty()) return;
            int kcal = QInputDialog::getInt(this, "Kalorie", "Ile kalorii (kcal):", 0, 0, 5000, 1, &ok);
            if (!ok) return;
            MealEntry* meal = new MealEntry(name, kcal);
            m_statsManager.addEntry(m_currentUser, meal); delete meal; refreshData();
            });
        return panel;
    }

    QWidget* createTrainingPanel() {
        auto panel = new QWidget();
        auto layout = new QVBoxLayout(panel);
        layout->setContentsMargins(40, 40, 40, 40);
        auto label = new QLabel("Twój Dziennik Treningowy (Dzisiaj)"); label->setProperty("class", "Header");
        trainingListWidget = new QListWidget(); trainingListWidget->setSelectionMode(QAbstractItemView::NoSelection);
        auto addBtn = new QPushButton("+ Dodaj Trening"); addBtn->setCursor(Qt::PointingHandCursor); addBtn->setMinimumHeight(50);
        layout->addWidget(label); layout->addWidget(trainingListWidget); layout->addWidget(addBtn);

        connect(addBtn, &QPushButton::clicked, [this]() {
            bool ok;
            QString name = QInputDialog::getText(this, "Nowy Trening", "Jakie ćwiczenie?", QLineEdit::Normal, "", &ok);
            if (!ok || name.isEmpty()) return;
            int mins = QInputDialog::getInt(this, "Czas", "Czas trwania (min):", 30, 1, 600, 1, &ok);
            if (!ok) return;
            int burned = QInputDialog::getInt(this, "Spalone", "Spalone kalorie (kcal):", 100, 0, 5000, 1, &ok);
            if (!ok) return;
            WorkoutEntry* workout = new WorkoutEntry(name, mins, burned);
            m_statsManager.addEntry(m_currentUser, workout); delete workout; refreshData();
            });
        return panel;
    }

    QStackedWidget* getStack() { return m_contentStack; }
};