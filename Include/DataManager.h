#pragma once
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDate>
#include "User.h"
#include "Health.h"

class DataManager {
    inline static const QString USERS_FILE = "Resource/users.txt";

public:
    static void init() {
        if (!QDir("Resource").exists()) QDir().mkpath("Resource");
    }

    static QString getUserDataPath(const QString& username) {
        QString folderName = username;
        if (folderName.compare("admin", Qt::CaseInsensitive) == 0) folderName = "#Admin";
        QString path = "Resource/Users/" + folderName + "/Data";
        QDir().mkpath(path);
        return path;
    }

    static User* login(const QString& login, const QString& pass) {
        QFile file(USERS_FILE);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList parts = line.split("|");
                if (parts.size() >= 3 && parts[0] == login && parts[1] == pass) {
                    return new AthleteUser(parts[0], parts[1], parts[2]);
                }
            }
        }
        if (login == "admin" && pass == "admin") return new AthleteUser("admin", "admin", "Administrator");
        return nullptr;
    }

    // --- ZAPISYWANIE PROFILU (BMI, DIETA, TRENINGI) ---
    // Dodaliśmy nowy argument: trainingGoal
    static void saveFullProfile(const QString& username, const BMIRecord& record, int calorieGoal, int trainingGoal) {
        QString path = getUserDataPath(username);

        // 1. Zapis logu BMI
        QFile bmiFile(path + "/bmi_logs.txt");
        if (bmiFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&bmiFile);
            out << record.serialize() << "\n";
        }

        // 2. Zapis celu kalorycznego
        QFile dietFile(path + "/diet_goal.txt");
        if (dietFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&dietFile);
            out << calorieGoal;
        }

        // 3. Zapis celu treningowego (NOWOŚĆ)
        QFile trainFile(path + "/training_goal.txt");
        if (trainFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&trainFile);
            out << trainingGoal;
        }
    }

    // --- ODCZYTYWANIE ---

    static int loadCalorieGoal(const QString& username) {
        QFile file(getUserDataPath(username) + "/diet_goal.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return QTextStream(&file).readLine().toInt();
        }
        return 2000; // Domyślna
    }

    // Nowa funkcja do odczytu celu treningowego
    static int loadTrainingGoal(const QString& username) {
        QFile file(getUserDataPath(username) + "/training_goal.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return QTextStream(&file).readLine().toInt();
        }
        return 3; // Domyślna
    }

    static int loadConsumedCalories(const QString& username) {
        return 0; // Mock (na razie 0)
    }

    static int loadWeeklyTrainings(const QString& username) {
        return 0; // Mock (na razie 0)
    }

    static void getBestStats(const QString& username, double& bestWeight, double& bestBMI) {
        QFile file(getUserDataPath(username) + "/bmi_logs.txt");
        bestWeight = 0.0;
        bestBMI = 0.0;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            double minDiff = 999.0;
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList parts = line.split("|");
                if (parts.size() >= 4) {
                    double w = parts[1].toDouble();
                    double bmi = parts[3].toDouble();
                    if (bestWeight == 0 || w < bestWeight) bestWeight = w;
                    double diff = std::abs(bmi - 22.0);
                    if (diff < minDiff) {
                        minDiff = diff;
                        bestBMI = bmi;
                    }
                }
            }
        }
    }
};