#pragma once
#include <QString>
#include <QDateTime>

// Klasa abstrakcyjna
class HealthMetric {
public:
    virtual ~HealthMetric() = default;
    virtual double calculate() const = 0;
    virtual QString getCategory() const = 0;
};

// Logika obliczania BMI (tylko dane, bez okienek!)
class BMIRecord : public HealthMetric {
private:
    double m_weight;
    double m_height;
    QDateTime m_timestamp;

public:
    BMIRecord(double w, double h)
        : m_weight(w), m_height(h), m_timestamp(QDateTime::currentDateTime()) {
    }

    double calculate() const override {
        if (m_height <= 0) return 0;
        double heightInMeters = m_height / 100.0;
        return m_weight / (heightInMeters * heightInMeters);
    }

    QString getCategory() const override {
        double val = calculate();
        if (val < 18.5) return "Niedowaga";
        if (val < 25.0) return "Norma";
        if (val < 30.0) return "Nadwaga";
        return "Otyłość";
    }

    QString serialize() const {
        return m_timestamp.toString(Qt::ISODate) + "|" +
            QString::number(m_weight) + "|" +
            QString::number(m_height) + "|" +
            QString::number(calculate());
    }
};