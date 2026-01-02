#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "User.h"

class Login : public QWidget {
    Q_OBJECT
private:
    QLineEdit* m_loginField;
    QLineEdit* m_passField;
    QLabel* m_statusLabel;

public:
    explicit Login(QWidget* parent = nullptr);

signals:
    void loginSuccess(User* user);

private slots:
    void handleLogin();
};