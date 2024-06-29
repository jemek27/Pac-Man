//
// Created by Ema on 10.06.2024.
//

#include "NumberDisplay.h"

NumberDisplay::NumberDisplay(const sf::Font& font, int fontSize, std::pair<int, int> position) : Font(font) {
    text.setFont(Font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.first, position.second);
}
