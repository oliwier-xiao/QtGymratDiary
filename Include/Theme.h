#pragma once
#include <QString>

namespace Theme {
    const QString APP_STYLE = R"(
        /* Globalne ustawienia */
        QMainWindow, QDialog, QMessageBox { background-color: #FFFFFF; }
        QWidget { font-family: 'Segoe UI', sans-serif; font-size: 14px; color: #000000; }
        
        QWidget#LoginScreen { background-color: #FFFFFF; }

        /* --- DOMYŚLNE PRZYCISKI (Fioletowe) --- */
        QPushButton {
            background-color: #6200EE; color: white; border-radius: 4px; 
            padding: 10px 20px; font-weight: bold; border: none; font-size: 14px;
        }
        QPushButton:hover { background-color: #3700B3; }

        /* --- NOWOŚĆ: CZERWONY PRZYCISK X (DO USUWANIA) --- */
        QPushButton#DeleteButton {
            background-color: #FF0000; /* Czerwony */
            color: white;
            font-weight: bold;
            font-size: 16px;
            border-radius: 5px;
            min-width: 30px;  /* Stała szerokość */
            max-width: 30px;
            min-height: 30px; /* Stała wysokość */
            max-height: 30px;
            padding: 0px;     /* Zero marginesu w środku */
        }
        QPushButton#DeleteButton:hover {
            background-color: #B00020; /* Ciemniejszy czerwony */
        }

        /* --- MENU BOCZNE --- */
        QFrame#SideMenu { background-color: #6200EE; border: none; }
        QPushButton#NavButton {
            background-color: transparent; color: #E0E0E0; border: none;
            text-align: left; padding: 15px; font-weight: bold; font-size: 16px; border-left: 5px solid transparent;
        }
        QPushButton#NavButton:hover { background-color: #4F00C2; color: white; }
        QPushButton#NavButton:checked { background-color: #3700B3; color: #03DAC6; border-left: 5px solid #03DAC6; }

        /* --- ELEMENTY UI --- */
        QFrame#Card { background-color: #F5F5F5; border: 1px solid #E0E0E0; border-radius: 12px; }
        QLabel[class="Header"] { color: #6200EE; font-size: 22px; font-weight: bold; margin-bottom: 10px; }

        /* LISTA (Poprawiona, żeby elementy były wyższe) */
        QListWidget {
            background-color: #FFFFFF; border: 2px solid #E0E0E0; border-radius: 8px;
            color: #000000; padding: 5px;
        }
        QListWidget::item {
            /* Zwiększamy odstępy, żeby X się zmieścił ładnie */
            border-bottom: 1px solid #F0F0F0;
            margin-bottom: 5px;
        }

        /* Inputy, Listy, Komunikaty - Reszta bez zmian */
        QLineEdit { padding: 8px; border: 2px solid #E0E0E0; border-radius: 4px; background: #FFFFFF; color: #000000; selection-background-color: #6200EE; selection-color: white; }
        QLineEdit:focus { border: 2px solid #03DAC6; }
        QInputDialog { background-color: #FFFFFF; color: #000000; }
        QInputDialog QLabel { color: #000000; font-size: 16px; }
        QSpinBox, QDoubleSpinBox { padding: 8px; border: 2px solid #E0E0E0; border-radius: 4px; background: #FFFFFF; color: #000000; selection-background-color: #6200EE; }
        QComboBox { padding: 8px; border: 2px solid #E0E0E0; border-radius: 4px; background: #FFFFFF; color: #000000; }
        QComboBox::drop-down { border: none; }
        QComboBox::down-arrow { image: none; border-left: 5px solid transparent; border-right: 5px solid transparent; border-top: 5px solid #6200EE; margin-right: 10px; }
        QComboBox QAbstractItemView { background-color: #FFFFFF; color: #000000; selection-background-color: #6200EE; selection-color: #FFFFFF; border: 1px solid #E0E0E0; }
        QMessageBox QPushButton { background-color: #6200EE; color: white; }
        QMessageBox QLabel { color: #000000; }
        QLabel[class="Error"] { color: #B00020; font-weight: bold; }
    )";
}