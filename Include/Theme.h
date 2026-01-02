#pragma once
#include <QString>

namespace Theme {
    const QString APP_STYLE = R"(
        QMainWindow { background-color: #FFFFFF; }
        QWidget { font-family: 'Segoe UI', sans-serif; font-size: 14px; }
        
        /* --- NOWOŒÆ: T³o dla ekranu logowania --- */
        QWidget#LoginScreen {
            background-color: #FFFFFF;
        }

        /* Navigation */
        QFrame#SideMenu { background-color: #6200EE; border: none; }
        
        QPushButton#NavButton {
            color: #E0E0E0;
            background-color: transparent; 
            border: none;
            text-align: left; 
            padding: 15px; 
            font-weight: bold; 
            font-size: 16px;
            border-left: 5px solid transparent;
        }
        QPushButton#NavButton:hover { background-color: #4F00C2; color: white; }
        QPushButton#NavButton:checked { background-color: #3700B3; color: #03DAC6; border-left: 5px solid #03DAC6; }

        /* Karty */
        QFrame#Card { 
            background-color: #F5F5F5; 
            border: 1px solid #E0E0E0; 
            border-radius: 12px; 
        }

        /* Inputy */
        QLineEdit { 
            padding: 8px; border: 2px solid #E0E0E0; border-radius: 4px; 
            background: #FFFFFF; color: #000000; 
            selection-background-color: #6200EE; selection-color: white;
        }
        QLineEdit:focus { border: 2px solid #03DAC6; }

        /* --- PRZYCISKI --- */
        QPushButton#PrimaryButton {
            background-color: #6200EE;  
            color: white;               
            border-radius: 4px; 
            padding: 10px 20px; 
            font-weight: bold;
            border: none;
            font-size: 14px;
        }
        QPushButton#PrimaryButton:hover { 
            background-color: #3700B3; 
        }

        /* Alerty */
        QLabel { color: #000000; } 
    )";
}