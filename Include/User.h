#pragma once
#include <QString>
#include <QDate>

// Klasa Abstrakcyjna
class User {
protected:
    QString login;
    QString password; // W produkcji powinno byæ hashowane!
    QString name;
    QDate creationDate;

public:
    User(const QString& l, const QString& p, const QString& n)
        : login(l), password(p), name(n), creationDate(QDate::currentDate()) {
    }
    virtual ~User() = default;

    virtual QString getRole() const = 0; // Metoda czysto wirtualna

    // Gettery (Hermetyzacja)
    QString getLogin() const { return login; }
    bool verifyPassword(const QString& input) const { return password == input; }
    QString getName() const { return name; }
    virtual QString serialize() const { return login + "|" + password + "|" + name + "|" + creationDate.toString(Qt::ISODate); }
};

// Konkretna implementacja
class AthleteUser : public User {
public:
    AthleteUser(const QString& l, const QString& p, const QString& n) : User(l, p, n) {}
    QString getRole() const override { return "Athlete"; }
};