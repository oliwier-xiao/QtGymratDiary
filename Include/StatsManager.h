#pragma once
#include <QString>
#include <QVector>
#include <QDate>
#include "DataManager.h"
#include "DiarySystem.h"

// Struktura pomocnicza - paczka z danymi na dziś
struct DailyStats {
    int caloriesConsumed = 0;
    int caloriesBurned = 0;
    int currentBalance = 0;
    int trainingsCount = 0;
    QVector<DiaryEntry*> todayEntries; // Lista obiektów z dzisiaj
};

class StatsManager {
public:
    // Metoda 1: Pobierz statystyki na dziś (Logika wyjęta z Dashboard)
    DailyStats getDailyStats(const QString& username) {
        DailyStats stats;

        // Pobieramy wszystko z bazy używając DataManagera
        QVector<DiaryEntry*> allEntries = DataManager::loadEntries(username);

        for (DiaryEntry* entry : allEntries) {
            // Filtrujemy tylko dzisiejsze
            if (entry->getDate().date() == QDate::currentDate()) {

                // Dodajemy do listy "na dziś"
                stats.todayEntries.append(entry);

                // Liczymy bilanse
                int change = entry->getCaloriesChange();
                stats.currentBalance += change;

                if (entry->getType() == "WORKOUT") {
                    stats.caloriesBurned += (-change);
                    stats.trainingsCount++;
                }
                else {
                    stats.caloriesConsumed += change;
                }
            }
            else {
                // Jeśli nie dzisiejszy, usuwamy go z pamięci
                delete entry;
            }
        }
        return stats;
    }

    // Metoda 2: Delegacja dodawania do DataManager
    void addEntry(const QString& username, DiaryEntry* entry) {
        DataManager::saveEntry(username, entry);
    }

    // Metoda 3: Delegacja usuwania do DataManager
    void removeEntry(const QString& username, const QString& serializedData) {
        DataManager::removeEntry(username, serializedData);
    }

    // Metody pomocnicze do celów
    int getCalorieGoal(const QString& username) {
        return DataManager::loadCalorieGoal(username);
    }

    int getTrainingGoal(const QString& username) {
        int goal = DataManager::loadTrainingGoal(username);
        return (goal == 0) ? 1 : goal;
    }
};