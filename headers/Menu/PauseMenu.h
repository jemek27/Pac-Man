//
// Created by jemek on 20.06.2024.
//

#ifndef PROJEKT_PAUSEMENU_H
#define PROJEKT_PAUSEMENU_H
#include "Menu.h"


class PauseMenu : public Menu {
    sf::RenderWindow* Window;
    bool GamePaused;
    bool InGameplay;

    friend class Game;
public:
    PauseMenu(sf::RenderWindow* window, const sf::Font& font);
    void checkPause();
    void pauseScreen();
};


#endif //PROJEKT_PAUSEMENU_H
