//
// Created by jemek on 20.06.2024.
//

#include "Menu/PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow *window, const sf::Font &font)
                                        : Menu(font), Window(window), GamePaused(false), InGameplay(true){
    addButton("Resume");
    addButton("Exit");
}

void PauseMenu::checkPause() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) or sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        GamePaused = true;
        pauseScreen();
    }
}

void PauseMenu::pauseScreen() {
    if(ItemSelected()) {
        switch(getSelectedItemIndex()) {
            case 0:
                GamePaused = false;
                break;
            case 1:
                InGameplay = false;
                GamePaused = false;
                sf::sleep(sf::milliseconds(100));
                break;
        }
    }
}
