#pragma once
#include <QString>

namespace Theme {
    const QString APP_STYLE = R"(
        QMainWindow { background-color: #FFFFFF; }
        QWidget { font-family: 'Segoe UI', sans-serif; font-size: 14px; }
        
        /* Navigation */
        QFrame#SideMenu { background-color: #6200EE; border: none; }
        
        QPushButton#NavButton {
            color: #E0E0E0; /* Lekko szary nieaktywny */
            background-color: transparent; 
            border: none;
            text-align: left; 
            padding: 15px; 
            font-weight: bold; 
            font-size: 16px;
            border-left: 5px solid transparent; /* Rezerwacja miejsca na pasek */
        }
        
        QPushButton#NavButton:hover { 
            background-color: #4F00C2; 
            color: white;
        }

        QPushButton#NavButton:checked { 
            background-color: #3700B3; 
            color: #03DAC6; /* Turkusowy tekst */
            border-left: 5px solid #03DAC6; /* Turkusowy pasek z boku */
        }

        /* Cards & Inputs */
        QFrame.Card { background-color: #F5F5F5; border-radius: 8px; border: 1px solid #E0E0E0; }
        QLabel.Header { color: #6200EE; font-size: 22px; font-weight: bold; margin-bottom: 10px; }
        
        QLineEdit { 
            padding: 8px; border: 2px solid #E0E0E0; border-radius: 4px; 
            background: #FFFFFF; color: #000000; 
            selection-background-color: #6200EE; selection-color: white;
        }
        QLineEdit:focus { border: 2px solid #03DAC6; }

        /* ComboBox (Nowy styl dla list rozwijanych) */
        QComboBox {
            padding: 8px; border: 2px solid #E0E0E0; border-radius: 4px; 
            background: #FFFFFF; color: #000000;
        }
        QComboBox::drop-down { border: none; }
        QComboBox::down-arrow { image: none; border-left: 5px solid transparent; border-right: 5px solid transparent; border-top: 5px solid #6200EE; margin-right: 10px; }
        
        /* Buttons */
        QPushButton.Primary {
            background-color: #6200EE; color: white; border-radius: 4px; padding: 10px 20px; font-weight: bold;
        }
        QPushButton.Primary:hover { background-color: #3700B3; }
        
        QPushButton.Secondary {
            background-color: #03DAC6; color: black; border-radius: 4px; padding: 10px 20px; font-weight: bold;
        }
        QPushButton.Secondary:hover { background-color: #018786; }

        /* Alerts */
        QLabel.Error { color: #B00020; font-weight: bold; }
    )";
}