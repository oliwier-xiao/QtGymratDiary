#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QResizeEvent>
#include "User.h"

class Login : public QWidget {
    Q_OBJECT

        QStackedWidget* m_stack;
    QLineEdit* m_loginField, * m_passField;
    QLabel* m_statusLabel;

    QLineEdit* m_regName, * m_regLogin, * m_regPass;
    QLabel* m_regStatus;

public:
    explicit Login(QWidget* parent = nullptr);

protected:
    // Wymuszamy wyœrodkowanie przy ka¿dej zmianie rozmiaru
    void resizeEvent(QResizeEvent* event) override {
        QWidget::resizeEvent(event);
        if (m_stack) {
            // Ustawiamy sztywny rozmiar kontenera logowania
            m_stack->setFixedSize(450, 550);
            // Centrujemy go w oknie
            m_stack->move(
                (width() - m_stack->width()) / 2,
                (height() - m_stack->height()) / 2
            );
        }
    }

private:
    QWidget* createLoginView();
    QWidget* createRegisterView();

private slots:
    void handleLogin();
    void handleRegister();
    void showRegister();
    void showLogin();

signals:
    void loginSuccess(User* user);
};