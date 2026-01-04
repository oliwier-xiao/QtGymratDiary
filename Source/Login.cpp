#include "UI/Login.h"
#include "UI/MainWindow.h"
#include "Core/DataManager.h"
#include "Core/Theme.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

Login::Login(QWidget* parent) : QWidget(parent) {
    // 1. Ustawienia okna
    setFixedSize(450, 650);
    setWindowTitle("Gymrat Diary - Logowanie");
    setStyleSheet(Theme::APP_STYLE); // Używamy stylu z Theme.h

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // 2. Kontener (Karta)
    auto card = new QFrame();
    card->setObjectName("Card");
    card->setFixedWidth(380);

    auto layout = new QVBoxLayout(card);
    layout->setSpacing(15);
    layout->setContentsMargins(40, 40, 40, 40);

    // 3. Logo
    auto logo = new QLabel();
    QPixmap p("Resource/logo.png");
    if (!p.isNull()) {
        logo->setPixmap(p.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logo->setAlignment(Qt::AlignCenter);
        layout->addWidget(logo);
    }

    auto title = new QLabel("GYMRAT DIARY");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #6200EE; margin-bottom: 10px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // 4. Pola tekstowe (lokalne zmienne)
    auto loginInput = new QLineEdit();
    loginInput->setPlaceholderText("Login");

    auto passInput = new QLineEdit();
    passInput->setPlaceholderText("Hasło");
    passInput->setEchoMode(QLineEdit::Password);

    // 5. Przyciski
    auto loginBtn = new QPushButton("Zaloguj się");
    loginBtn->setObjectName("PrimaryButton");
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setMinimumHeight(45);

    auto registerBtn = new QPushButton("Utwórz konto");
    registerBtn->setCursor(Qt::PointingHandCursor);
    // Styl "Outlined" dla rejestracji (biały z ramką)
    registerBtn->setStyleSheet("QPushButton { background: white; color: #6200EE; border: 2px solid #6200EE; font-weight: bold; margin-top: 10px; border-radius: 4px; } QPushButton:hover { background: #F3E5F5; }");
    registerBtn->setMinimumHeight(45);

    // 6. LOGIKA (LAMBDY) - Tutaj jest uproszczenie!

    // --- Logowanie ---
    connect(loginBtn, &QPushButton::clicked, [=]() {
        QString user = loginInput->text().trimmed();
        QString pass = passInput->text().trimmed();

        if (user.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(this, "Błąd", "Podaj login i hasło!");
            return;
        }

        User* loggedUser = DataManager::login(user, pass);
        if (loggedUser) {
            MainWindow* w = new MainWindow(loggedUser);
            w->show();
            this->close();
        }
        else {
            QMessageBox::critical(this, "Błąd", "Nieprawidłowy login lub hasło.");
        }
        });

    // --- Rejestracja ---
    connect(registerBtn, &QPushButton::clicked, [=]() {
        QString user = loginInput->text().trimmed();
        QString pass = passInput->text().trimmed();

        if (user.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(this, "Rejestracja", "Wpisz login i hasło w pola, a następnie kliknij Utwórz konto.");
            return;
        }

        bool ok;
        QString name = QInputDialog::getText(this, "Rejestracja", "Jak masz na imię?", QLineEdit::Normal, "", &ok);

        if (ok && !name.isEmpty()) {
            if (DataManager::registerUser(user, pass, name)) {
                QMessageBox::information(this, "Sukces", "Konto utworzone! Możesz się teraz zalogować.");
            }
            else {
                QMessageBox::warning(this, "Błąd", "Taki użytkownik już istnieje!");
            }
        }
        });

    // Obsługa Entera
    connect(loginInput, &QLineEdit::returnPressed, loginBtn, &QPushButton::click);
    connect(passInput, &QLineEdit::returnPressed, loginBtn, &QPushButton::click);

    // Składanie widoku
    layout->addWidget(loginInput);
    layout->addWidget(passInput);
    layout->addSpacing(10);
    layout->addWidget(loginBtn);
    layout->addWidget(registerBtn);
    layout->addStretch();

    mainLayout->addWidget(card);
}