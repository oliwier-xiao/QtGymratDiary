#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include "../Core/User.h" 

class Login : public QWidget {
    Q_OBJECT

private:
    QLineEdit* m_usernameInput;
    QLineEdit* m_passwordInput;

public:
    explicit Login(QWidget* parent = nullptr);

private:
    QWidget* createLoginView();

private slots:
    void handleLogin();
    void handleRegister();

signals:
    // Zostawiamy ten sygna³ dla kompatybilnoœci, ¿eby kompilator nie krzycza³
    void loginSuccess(User* user);
};