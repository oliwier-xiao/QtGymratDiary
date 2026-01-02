#pragma once
#include <QString>
#include <QVector>

// Modu³ Treningowy
class Exercise {
private:
    QString m_name;
    int m_sets;
    int m_reps;
public:
    Exercise(QString n, int s, int r) : m_name(n), m_sets(s), m_reps(r) {}
    QString getName() const { return m_name; }
    QString getSummary() const { return m_name + ": " + QString::number(m_sets) + "x" + QString::number(m_reps); }
};

class Training {
private:
    QString m_title;
    QVector<Exercise> m_exercises;
public:
    explicit Training(QString t) : m_title(t) {}
    void addExercise(const Exercise& e) { m_exercises.push_back(e); }
    QString getTitle() const { return m_title; }
    const QVector<Exercise>& getExercises() const { return m_exercises; }
};

// Modu³ Diety
class FoodProduct {
private:
    QString m_name;
    int m_calories;
    double m_protein;
public:
    FoodProduct(QString n, int c, double p) : m_name(n), m_calories(c), m_protein(p) {}
    int getKcal() const { return m_calories; }
    QString getName() const { return m_name; }
};

class Meal {
private:
    QString m_mealName; // np. "Œniadanie"
    QVector<FoodProduct> m_products;
public:
    explicit Meal(QString name) : m_mealName(name) {}
    void addProduct(const FoodProduct& p) { m_products.push_back(p); }
    int totalCalories() const {
        int sum = 0;
        for (const auto& p : m_products) sum += p.getKcal();
        return sum;
    }
};