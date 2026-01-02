#pragma once
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "User.h"
#include "Health.h"

class DataManager {
    // Plik z bazą użytkowników trzymamy w głównym folderze Resource
    inline static const QString USERS_FILE = "Resource/users.txt";

public:
    static void init() {
        // Tworzymy główny folder Resource jeśli nie istnieje
        if (!QDir("Resource").exists()) {
            QDir().mkpath("Resource");
        }
    }

    // --- NOWA FUNKCJA: Generuje ścieżkę dla konkretnego usera ---
    static QString getUserDataPath(const QString& username) {
        QString folderName = username;

        // Logika dla Admina: Folder z hashem, żeby był na górze
        if (folderName.compare("admin", Qt::CaseInsensitive) == 0) {
            folderName = "#Admin";
        }

        // Budujemy ścieżkę: Resource/Users/Nazwa/Data
        QString path = "Resource/Users/" + folderName + "/Data";

        // Jeśli foldery nie istnieją -> stwórz je automatycznie
        QDir dir;
        if (!dir.exists(path)) {
            dir.mkpath(path);
        }
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
        // Backdoor dla Admina (nie wymaga pliku users.txt)
        if (login == "admin" && pass == "admin") {
            return new AthleteUser("admin", "admin", "Administrator");
        }
        return nullptr;
    }

    // --- ZMIANA: Przyjmujemy nazwę użytkownika, żeby wiedzieć gdzie zapisać ---
    static void saveBMILog(const QString& username, const BMIRecord& record) {
        QString path = getUserDataPath(username); // Pobieramy np. Resource/Users/#Admin/Data

        QFile file(path + "/bmi_logs.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << record.serialize() << "\n";
        }
    }
};