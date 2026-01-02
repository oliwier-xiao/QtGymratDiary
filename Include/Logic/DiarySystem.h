#pragma once
#include <QString>
#include <QDateTime>

class DiaryEntry {
protected:
    QDateTime m_timestamp;
    QString m_type;

public:
    DiaryEntry(const QString& type)
        : m_type(type), m_timestamp(QDateTime::currentDateTime()) {
    }

    virtual ~DiaryEntry() = default;

    virtual QString getDescription() const = 0;
    virtual int getCaloriesChange() const = 0;

    // --- NOWOŚĆ: Metoda do ustawiania daty (potrzebna przy wczytywaniu) ---
    void setDate(const QDateTime& dt) { m_timestamp = dt; }
    // ----------------------------------------------------------------------

    QDateTime getDate() const { return m_timestamp; }
    QString getType() const { return m_type; }

    virtual QString serialize() const {
        return m_timestamp.toString(Qt::ISODate) + "|" + m_type;
    }
};

class MealEntry : public DiaryEntry {
private:
    QString m_foodName;
    int m_calories;

public:
    MealEntry(const QString& name, int kcal)
        : DiaryEntry("MEAL"), m_foodName(name), m_calories(kcal) {
    }

    QString getDescription() const override {
        return "🍽️ " + m_foodName + " (" + QString::number(m_calories) + " kcal)";
    }

    int getCaloriesChange() const override { return m_calories; }

    QString serialize() const override {
        return DiaryEntry::serialize() + "|" + m_foodName + "|" + QString::number(m_calories);
    }
};

class WorkoutEntry : public DiaryEntry {
private:
    QString m_exerciseName;
    int m_durationMins;
    int m_burnedKcal;

public:
    WorkoutEntry(const QString& name, int mins, int burned)
        : DiaryEntry("WORKOUT"), m_exerciseName(name), m_durationMins(mins), m_burnedKcal(burned) {
    }

    QString getDescription() const override {
        return "💪 " + m_exerciseName + " (" + QString::number(m_durationMins) + " min) - Spalono: " + QString::number(m_burnedKcal) + " kcal";
    }

    int getCaloriesChange() const override { return -m_burnedKcal; }

    QString serialize() const override {
        return DiaryEntry::serialize() + "|" + m_exerciseName + "|" +
            QString::number(m_durationMins) + "|" + QString::number(m_burnedKcal);
    }
};