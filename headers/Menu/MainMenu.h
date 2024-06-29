//
// Created by jemek on 14.06.2024.
//

#ifndef PROJEKT_MAINMENU_H
#define PROJEKT_MAINMENU_H
#include "Menu.h"
#include "FileInputOutputManager.h"
#include <regex>

class MainMenu : public Menu {
    sf::RenderWindow* Window;
    sf::Event Event;
    sf::Vector2<unsigned int> WindowSize;

    FileInputOutputManager* FileManager;
    std::vector<std::pair<std::string, int>> Scoreboard;

    std::string Nickname;
    bool NickNameIsValid;
    bool InGameplay;

    friend class Game;
public:
    MainMenu(sf::RenderWindow* window, const sf::Font& font);
    ~MainMenu() override;
    void interact();
    void initiateNickINputScrean();
    void nicknameInput(const std::regex& nicknameRegex, sf::Text& prompt);
    void manageNewScore(const int& score);
    char checkPlace(const int& score);
    void viewScoreboard();
    void viewEndScore(const int& score);
    void poolEvent();
};


#endif //PROJEKT_MAINMENU_H
