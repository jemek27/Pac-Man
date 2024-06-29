//
// Created by Ema on 10.06.2024.
//

#ifndef PROJEKT_MENU_H
#define PROJEKT_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

class Menu {
protected:
    std::vector<sf::Text> menuButtons;
    std::vector<sf::Text> Text;
    sf::Font Font;
    int selectedItemIndex;
public:
    Menu(const sf::Font& font);
    virtual ~Menu() = default;

    void addButton(const std::string& label);
    void addText(const std::string& label, const sf::Vector2f& position);
    void addText(sf::Text text);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedItemIndex() const;
    bool ItemSelected();

};

#endif //PROJEKT_MENU_H
