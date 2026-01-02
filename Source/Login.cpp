#include "Login.h"
#include "DataManager.h"
#include <QMessageBox>
#include <QFrame>

Login::Login(QWidget* parent) : QWidget(parent) {
    // --- ZMIANA: Usunąłem setStyleSheet, który psuł przyciski! ---
    // Zamiast tego nadajemy nazwę, żeby Theme.h ustawił białe tło.
    setObjectName("LoginScreen");

    m_stack = new QStackedWidget(this);
    m_stack->addWidget(createLoginView());
    m_stack->addWidget(createRegisterView());

    // Wymuszamy rozmiar startowy
    resize(1200, 800);
}

QWidget* Login::createLoginView() {
    auto container = new QFrame();
    container->setObjectName("Card");

    auto layout = new QVBoxLayout(container);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(15);

    auto title = new QLabel("GYMRAT DIARY");
    title->setStyleSheet("font-size: 28px; font-weight: bold; color: #6200EE; margin-bottom: 20px; border: none; background: transparent;");
    title->setAlignment(Qt::AlignCenter);

    m_loginField = new QLineEdit(); m_loginField->setPlaceholderText("Login");
    m_passField = new QLineEdit(); m_passField->setPlaceholderText("Hasło");
    m_passField->setEchoMode(QLineEdit::Password);

    auto loginBtn = new QPushButton("ZALOGUJ SIĘ");
    loginBtn->setObjectName("PrimaryButton");
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setMinimumHeight(45);

    auto registerBtn = new QPushButton("Utwórz nowe konto");
    registerBtn->setObjectName("PrimaryButton");
    registerBtn->setCursor(Qt::PointingHandCursor);
    registerBtn->setMinimumHeight(45);

    m_statusLabel = new QLabel("");
    m_statusLabel->setStyleSheet("color: #B00020; font-weight: bold; border: none; background: transparent;");
    m_statusLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(title);
    layout->addSpacing(10);
    layout->addWidget(m_loginField);
    layout->addWidget(m_passField);
    layout->addSpacing(20);
    layout->addWidget(loginBtn);

    auto labelOr = new QLabel("— lub —");
    labelOr->setAlignment(Qt::AlignCenter);
    labelOr->setStyleSheet("color: #888; margin: 5px 0; border: none; background: transparent;");
    layout->addWidget(labelOr);

    layout->addWidget(registerBtn);
    layout->addWidget(m_statusLabel);
    layout->addStretch();

    connect(loginBtn, &QPushButton::clicked, this, &Login::handleLogin);
    connect(m_loginField, &QLineEdit::returnPressed, this, &Login::handleLogin);
    connect(m_passField, &QLineEdit::returnPressed, this, &Login::handleLogin);
    connect(registerBtn, &QPushButton::clicked, this, &Login::showRegister);

    return container;
}

QWidget* Login::createRegisterView() {
    auto container = new QFrame();
    container->setObjectName("Card");

    auto layout = new QVBoxLayout(container);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(15);

    auto title = new QLabel("REJESTRACJA");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #03DAC6; margin-bottom: 20px; border: none; background: transparent;");
    title->setAlignment(Qt::AlignCenter);

    m_regName = new QLineEdit(); m_regName->setPlaceholderText("Twoje Imię (np. Oliwier)");
    m_regLogin = new QLineEdit(); m_regLogin->setPlaceholderText("Wymyśl Login");
    m_regPass = new QLineEdit(); m_regPass->setPlaceholderText("Wymyśl Hasło");
    m_regPass->setEchoMode(QLineEdit::Password);

    auto createBtn = new QPushButton("ZAREJESTRUJ I ZALOGUJ");
    createBtn->setObjectName("PrimaryButton");
    createBtn->setMinimumHeight(45);
    createBtn->setCursor(Qt::PointingHandCursor);

    auto backBtn = new QPushButton("Anuluj");
    backBtn->setFlat(true);
    backBtn->setStyleSheet("color: #666; font-weight: bold; border: none; background: transparent;");
    backBtn->setCursor(Qt::PointingHandCursor);

    m_regStatus = new QLabel("");
    m_regStatus->setStyleSheet("color: #B00020; font-weight: bold; border: none; background: transparent;");
    m_regStatus->setAlignment(Qt::AlignCenter);

    layout->addWidget(title);
    layout->addWidget(m_regName);
    layout->addWidget(m_regLogin);
    layout->addWidget(m_regPass);
    layout->addSpacing(20);
    layout->addWidget(createBtn);
    layout->addWidget(m_regStatus);
    layout->addWidget(backBtn);
    layout->addStretch();

    connect(createBtn, &QPushButton::clicked, this, &Login::handleRegister);
    connect(backBtn, &QPushButton::clicked, this, &Login::showLogin);

    return container;
}

void Login::handleLogin() {
    QString login = m_loginField->text();
    QString pass = m_passField->text();
    User* user = DataManager::login(login, pass);
    if (user) {
        m_statusLabel->setText("Logowanie...");
        emit loginSuccess(user);
    }
    else {
        m_statusLabel->setText("Błędny login lub hasło!");
        m_passField->clear();
    }
}

void Login::handleRegister() {
    QString name = m_regName->text();
    QString login = m_regLogin->text();
    QString pass = m_regPass->text();
    if (name.isEmpty() || login.isEmpty() || pass.isEmpty()) {
        m_regStatus->setText("Wypełnij wszystkie pola!");
        return;
    }
    if (DataManager::registerUser(login, pass, name)) {
        QMessageBox::information(this, "Sukces", "Konto " + login + " zostało utworzone!");
        showLogin();
        m_loginField->setText(login);
        m_passField->setFocus();
    }
    else {
        m_regStatus->setText("Login zajęty! Wybierz inny.");
    }
}
void Login::showRegister() { m_stack->setCurrentIndex(1); m_regStatus->clear(); m_regName->clear(); m_regLogin->clear(); m_regPass->clear(); }
void Login::showLogin() { m_stack->setCurrentIndex(0); m_statusLabel->clear(); }