#pragma once
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDate>
#include <QVector>
#include <cmath>

#include "User.h"
#include "Health.h"
#include "DiarySystem.h"

class DataManager {
    inline static const QString USERS_FILE = "Resource/users.txt";

public:
    static void init() {
        if (!QDir("Resource").exists()) QDir().mkpath("Resource");
        QFile file(USERS_FILE);
        if (!file.exists()) {
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) file.close();
        }
    }

    static QString getUserDataPath(const QString& username) {
        QString folderName = username;
        if (folderName.compare("admin", Qt::CaseInsensitive) == 0) folderName = "#Admin";
        QString path = "Resource/Users/" + folderName + "/Data";
        if (!QDir(path).exists()) QDir().mkpath(path);
        return path;
    }

    static bool registerUser(const QString& login, const QString& pass, const QString& name) {
        QFile rFile(USERS_FILE);
        if (rFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&rFile);
            while (!in.atEnd()) {
                if (in.readLine().startsWith(login + "|")) return false;
            }
            rFile.close();
        }

        QFile wFile(USERS_FILE);
        if (wFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&wFile);
            out << login << "|" << pass << "|" << name << "|" << QDate::currentDate().toString(Qt::ISODate) << "\n";
            wFile.close();
            getUserDataPath(login);
            return true;
        }
        return false;
    }

    static User* login(const QString& login, const QString& pass) {
        QFile file(USERS_FILE);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QStringList parts = in.readLine().split("|");
                if (parts.size() >= 3 && parts[0] == login && parts[1] == pass) {
                    return new AthleteUser(parts[0], parts[1], parts[2]);
                }
            }
        }
        if (login == "admin" && pass == "admin") return new AthleteUser("admin", "admin", "Administrator");
        return nullptr;
    }

    static void saveFullProfile(const QString& username, const BMIRecord& record, int calorieGoal, int trainingGoal) {
        QString path = getUserDataPath(username);
        QFile bmiFile(path + "/bmi_logs.txt");
        if (bmiFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&bmiFile); out << record.serialize() << "\n";
        }
        QFile dietFile(path + "/diet_goal.txt");
        if (dietFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&dietFile); out << calorieGoal;
        }
        QFile trainFile(path + "/training_goal.txt");
        if (trainFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&trainFile); out << trainingGoal;
        }
    }

    static int loadCalorieGoal(const QString& username) {
        QFile file(getUserDataPath(username) + "/diet_goal.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) return QTextStream(&file).readLine().toInt();
        return 2000;
    }

    static int loadTrainingGoal(const QString& username) {
        QFile file(getUserDataPath(username) + "/training_goal.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) return QTextStream(&file).readLine().toInt();
        return 3;
    }

    static void getBestStats(const QString& username, double& bestWeight, double& bestBMI) {
        QFile file(getUserDataPath(username) + "/bmi_logs.txt");
        bestWeight = 0.0; bestBMI = 0.0;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            double minDiff = 999.0;
            while (!in.atEnd()) {
                QStringList parts = in.readLine().split("|");
                if (parts.size() >= 4) {
                    double w = parts[1].toDouble();
                    double bmi = parts[3].toDouble();
                    if (bestWeight == 0 || w < bestWeight) bestWeight = w;
                    double diff = std::abs(bmi - 22.0);
                    if (diff < minDiff) { minDiff = diff; bestBMI = bmi; }
                }
            }
        }
    }

    static void saveEntry(const QString& username, const DiaryEntry* entry) {
        QString path = getUserDataPath(username);
        QFile file(path + "/diary_log.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file); out << entry->serialize() << "\n";
        }
    }

    // --- POPRAWIONA FUNKCJA WCZYTYWANIA ---
    static QVector<DiaryEntry*> loadEntries(const QString& username) {
        QVector<DiaryEntry*> entries;
        QString path = getUserDataPath(username);
        QFile file(path + "/diary_log.txt");

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList parts = line.split("|");

                if (parts.size() >= 2) {
                    // 1. ODCZYTUJEMY DATĘ Z PLIKU
                    QDateTime dt = QDateTime::fromString(parts[0], Qt::ISODate);
                    QString type = parts[1];

                    DiaryEntry* entry = nullptr;

                    if (type == "MEAL" && parts.size() >= 4) {
                        entry = new MealEntry(parts[2], parts[3].toInt());
                    }
                    else if (type == "WORKOUT" && parts.size() >= 5) {
                        entry = new WorkoutEntry(parts[2], parts[3].toInt(), parts[4].toInt());
                    }

                    if (entry) {
                        // 2. USTAWIAMY TĘ DATĘ W OBIEKCIE (ważne dla usuwania!)
                        entry->setDate(dt);
                        entries.append(entry);
                    }
                }
            }
        }
        return entries;
    }

    static void removeEntry(const QString& username, const QString& serializedData) {
        QString path = getUserDataPath(username) + "/diary_log.txt";
        QFile file(path);
        QStringList lines;
        bool found = false;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                // Porównujemy dokładnie treść linii
                if (!found && line == serializedData) {
                    found = true;
                }
                else {
                    lines.append(line);
                }
            }
            file.close();
        }

        if (found) {
            if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                QTextStream out(&file);
                for (const QString& l : lines) out << l << "\n";
            }
        }
    }

    static int loadConsumedCalories(const QString& username) { return 0; }
    static int loadWeeklyTrainings(const QString& username) { return 0; }
};