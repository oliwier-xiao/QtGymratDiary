#pragma once
#include <QString>
#include <QVector>
#include <QRandomGenerator>

class Mentor {
public:
    static QString getResponse(QString input) {
        input = input.toLower(); // Ignorujemy wielkość liter (np. "Klatka" -> "klatka")

        // --- 1. MOTYWACJA ---
        if (input.contains("motywacj") || input.contains("nie chce mi sie") || input.contains("ciężko") || input.contains("trudno")) {
            QVector<QString> quotes = {
                "„Nie musisz być wielki, żeby zacząć, ale musisz zacząć, żeby być wielki.”",
                "„Twój jedyny limit to Ty sam.”",
                "„Ból, który czujesz dzisiaj, to siła, którą poczujesz jutro.”",
                "„Nie poddawaj się. Cierp teraz i żyj resztę życia jako mistrz.” – Muhammad Ali",
                "„Każdy trening to krok bliżej do celu. Nie zmarnuj go!”,",
                "„Pamiętaj, dlaczego zacząłeś. Nie rezygnuj w połowie drogi.”"
            };
            return quotes[QRandomGenerator::global()->bounded(quotes.size())];
        }

        // --- 2. TRENINGI (Top 3) ---
        if (input.contains("klat") || input.contains("piersiow")) {
            return "🏆 **Top 3 na Klatkę:**\n1. Wyciskanie sztangi na ławce poziomej\n2. Wyciskanie hantli na skosie (+)\n3. Rozpiętki (brama lub hantle)";
        }
        if (input.contains("plec")) {
            return "🏆 **Top 3 na Plecy:**\n1. Podciąganie na drążku (szeroki chwyt)\n2. Martwy Ciąg\n3. Wiosłowanie sztangą w opadzie";
        }
        if (input.contains("nog") || input.contains("przysiad")) {
            return "🏆 **Top 3 na Nogi:**\n1. Przysiady ze sztangą (Król ćwiczeń!)\n2. Wykroki z hantlami\n3. Martwy ciąg na prostych nogach (RDL)";
        }
        if (input.contains("bark")) {
            return "🏆 **Top 3 na Barki:**\n1. Wyciskanie żołnierskie (OHP)\n2. Wznosy hantli bokiem\n3. Face Pulls (tył barku)";
        }
        if (input.contains("bic") || input.contains("łap")) {
            return "🏆 **Top 3 na Biceps:**\n1. Uginanie ramion ze sztangą prostą\n2. Uginanie z hantlami z supinacją\n3. Uginanie na modlitewniku";
        }
        if (input.contains("tric")) {
            return "🏆 **Top 3 na Triceps:**\n1. Wyciskanie wąsko\n2. Pompki na poręczach (Dipy)\n3. Prostowanie linek wyciągu";
        }

        // --- 3. DIETA ---
        if (input.contains("śniadani")) {
            QVector<QString> meals = {
                "🍳 Jajecznica z 3 jaj na maśle + pomidor i szczypiorek.",
                "🥣 Owsianka na mleku/wodzie z odżywką białkową i owocami leśnymi.",
                "🥞 Placuszki twarogowe z mąki ryżowej z masłem orzechowym."
            };
            return "Moja propozycja na śniadanie:\n" + meals[QRandomGenerator::global()->bounded(meals.size())];
        }
        if (input.contains("obiad") || input.contains("lunch")) {
            QVector<QString> meals = {
                "🍗 Kurczak z ryżem i brokułami (klasyk!).",
                "🍝 Makaron pełnoziarnisty z indykiem w sosie pomidorowym.",
                "🐟 Pieczony łosoś z ziemniakami i szparagami."
            };
            return "Moja propozycja na obiad:\n" + meals[QRandomGenerator::global()->bounded(meals.size())];
        }
        if (input.contains("kolacj")) {
            QVector<QString> meals = {
                "🥗 Sałatka z tuńczykiem, jajkiem i oliwą.",
                "🧀 Twaróg chudy z rzodkiewką i kromką chleba żytniego.",
                "🥤 Szejk kazeinowy lub serek wiejski z orzechami."
            };
            return "Moja propozycja na kolację:\n" + meals[QRandomGenerator::global()->bounded(meals.size())];
        }

        // --- 4. DEFAULT (Gdy nie zrozumie) ---
        return "Jestem Twoim Mentorem! Zapytaj mnie o:\n\n"
            "🏋️ **Trening:** np. 'ćwiczenia na klatę', 'plecy', 'nogi'\n"
            "🥗 **Dieta:** np. 'co na śniadanie?', 'obiad', 'kolacja'\n"
            "🔥 **Motywacja:** np. 'daj motywację', 'nie chce mi się'";
    }
};