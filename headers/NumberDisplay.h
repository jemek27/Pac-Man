//
// Created by Ema on 10.06.2024.
//

#ifndef PROJEKT_NUMBERDISPLAY_H
#define PROJEKT_NUMBERDISPLAY_H
#include <SFML/Graphics.hpp>
#include <iostream>

class NumberDisplay {
    sf::Font Font;
public:
    sf::Text text;
    NumberDisplay() = default;
    NumberDisplay(const sf::Font& font, int fontSize, std::pair<int, int> position);
};


#endif //PROJEKT_NUMBERDISPLAY_H
