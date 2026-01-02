#include "Login.h"
#include "DataManager.h"
#include <QMessageBox>
#include <QFrame>

Login::Login(QWidget* parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto container = new QFrame();
    container->setObjectName("Card");
    container->setFixedWidth(400);
    auto formLayout = new QVBoxLayout(container);

    auto title = new QLabel("GYMRAT DIARY");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #6200EE; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);

    m_loginField = new QLineEdit();
    m_loginField->setPlaceholderText("Login");

    m_passField = new QLineEdit();
    m_passField->setPlaceholderText("Hasło");
    m_passField->setEchoMode(QLineEdit::Password);

    auto loginBtn = new QPushButton("ZALOGUJ");
    loginBtn->setProperty("class", "Primary");

    m_statusLabel = new QLabel("");
    m_statusLabel->setStyleSheet("color: #B00020; font-weight: bold;");
    m_statusLabel->setAlignment(Qt::AlignCenter);

    formLayout->addWidget(title);
    formLayout->addWidget(m_loginField);
    formLayout->addWidget(m_passField);
    formLayout->addSpacing(15);
    formLayout->addWidget(loginBtn);
    formLayout->addWidget(m_statusLabel);

    layout->addWidget(container);

    // --- Obsługa przycisku ---
    connect(loginBtn, &QPushButton::clicked, this, &Login::handleLogin);

    // --- NOWOŚĆ: Obsługa klawisza ENTER ---
    connect(m_loginField, &QLineEdit::returnPressed, this, &Login::handleLogin);
    connect(m_passField, &QLineEdit::returnPressed, this, &Login::handleLogin);
}

void Login::handleLogin() {
    QString login = m_loginField->text();
    QString pass = m_passField->text();

    User* user = DataManager::login(login, pass);

    if (user) {
        m_statusLabel->setText("Zalogowano pomyślnie!");
        emit loginSuccess(user);
    }
    else {
        m_statusLabel->setText("Błędny login lub hasło!");
    }
}