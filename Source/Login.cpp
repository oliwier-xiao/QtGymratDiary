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
#include <QPixmap> 

Login::Login(QWidget* parent) : QWidget(parent) {
    this->setObjectName("LoginScreen");
    this->setFixedSize(450, 650);
    this->setWindowTitle("Gymrat Diary - Logowanie");

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(createLoginView());
    // Nakładamy styl globalny na to okno
    this->setStyleSheet(Theme::APP_STYLE);
}

QWidget* Login::createLoginView() {
    auto container = new QFrame();
    container->setObjectName("Card");
    container->setFixedWidth(380);

    auto layout = new QVBoxLayout(container);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(15);

    // --- LOGO ---
    auto logoLabel = new QLabel();
    QPixmap pixmap("Resource/logo.png");
    if (!pixmap.isNull()) {
        logoLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logoLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(logoLabel);
    }

    auto title = new QLabel("GYMRAT DIARY");
    title->setProperty("class", "Header");
    title->setAlignment(Qt::AlignCenter);
    // Styl nagłówka
    title->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 10px; color: #6200EE;");
    layout->addWidget(title);

    // --- POLA TEKSTOWE ---
    m_usernameInput = new QLineEdit();
    m_usernameInput->setPlaceholderText("Login");

    m_passwordInput = new QLineEdit();
    m_passwordInput->setPlaceholderText("Hasło");
    m_passwordInput->setEchoMode(QLineEdit::Password);

    // --- PRZYCISK LOGOWANIA (Primary) ---
    auto loginBtn = new QPushButton("Zaloguj się");
    loginBtn->setObjectName("PrimaryButton");
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setMinimumHeight(45);

    // --- PRZYCISK REJESTRACJI (Secondary / Outlined) ---
    auto registerBtn = new QPushButton("Utwórz konto");
    registerBtn->setCursor(Qt::PointingHandCursor);
    registerBtn->setMinimumHeight(45);

    // NAPRAWA: Jawnie ustawiamy tło na białe i dodajemy ramkę.
    // Dzięki temu tekst #6200EE będzie widoczny na białym tle #FFFFFF.
    registerBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FFFFFF;"   // Białe tło (kluczowe!)
        "  color: #6200EE;"              // Fioletowy tekst
        "  border: 2px solid #6200EE;"   // Fioletowa ramka
        "  border-radius: 4px;"
        "  font-weight: bold;"
        "  margin-top: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #EDE7F6;"   // Bardzo jasny fiolet po najechaniu
        "}"
        "QPushButton:pressed {"
        "  background-color: #D1C4E9;"   // Ciemniejszy przy kliknięciu
        "}"
    );

    layout->addWidget(m_usernameInput);
    layout->addWidget(m_passwordInput);
    layout->addSpacing(10);
    layout->addWidget(loginBtn);
    layout->addWidget(registerBtn);
    layout->addStretch();

    // Podpinanie sygnałów
    connect(loginBtn, &QPushButton::clicked, this, &Login::handleLogin);
    connect(registerBtn, &QPushButton::clicked, this, &Login::handleRegister);
    connect(m_usernameInput, &QLineEdit::returnPressed, this, &Login::handleLogin);
    connect(m_passwordInput, &QLineEdit::returnPressed, this, &Login::handleLogin);

    return container;
}

void Login::handleLogin() {
    QString user = m_usernameInput->text().trimmed();
    QString pass = m_passwordInput->text().trimmed();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Podaj login i hasło!");
        return;
    }

    User* loggedUser = DataManager::login(user, pass);

    if (loggedUser) {
        // Tworzymy główne okno i zamykamy ekran logowania
        MainWindow* w = new MainWindow(loggedUser);
        w->show();
        this->close();
    }
    else {
        QMessageBox::critical(this, "Błąd", "Nieprawidłowy login lub hasło.");
    }
}

void Login::handleRegister() {
    QString user = m_usernameInput->text().trimmed();
    QString pass = m_passwordInput->text().trimmed();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Rejestracja", "Wpisz login i hasło w pola powyżej, a następnie kliknij Utwórz konto.");
        return;
    }

    bool ok;
    // Upewnij się, że plik jest zapisany jako UTF-8 z BOM, żeby polskie znaki działały!
    QString name = QInputDialog::getText(this, "Rejestracja", "Podaj swoje imię:", QLineEdit::Normal, "", &ok);

    if (!ok || name.isEmpty()) return;

    if (DataManager::registerUser(user, pass, name)) {
        QMessageBox::information(this, "Sukces", "Konto utworzone! Możesz się teraz zalogować.");
    }
    else {
        QMessageBox::warning(this, "Błąd", "Taki użytkownik już istnieje!");
    }
}