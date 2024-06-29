//
// Created by Ema on 10.06.2024.
//

#include "Menu/Menu.h"

Menu::Menu(const sf::Font& font) : selectedItemIndex(0), Font(font) {}

void Menu::addButton(const std::string& label) {
    sf::Text text;
    text.setFont(Font);
    text.setString(label);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setPosition(sf::Vector2f(100, 100 + menuButtons.size() * 50));
    menuButtons.push_back(text);
}

void Menu::addText(const std::string& label, const sf::Vector2f& position) {
    sf::Text text;
    text.setFont(Font);
    text.setString(label);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setPosition(position);
    Text.push_back(text);
}

void Menu::draw(sf::RenderWindow& window) {
    for (size_t i = 0; i < menuButtons.size(); i++) {
        if (i == selectedItemIndex) {
            menuButtons[i].setFillColor(sf::Color::Red);
        } else {
            menuButtons[i].setFillColor(sf::Color::White);
        }
        window.draw(menuButtons[i]);
    }

    for (const auto& t : Text) {
        window.draw(t);
    }
}

void Menu::moveUp() {
    if (selectedItemIndex - 1 >= 0) {
        selectedItemIndex--;
    }
}

void Menu::moveDown() {
    if (selectedItemIndex + 1 < menuButtons.size()) {
        selectedItemIndex++;
    }
}

int Menu::getSelectedItemIndex() const {
    return selectedItemIndex;
}

bool Menu::ItemSelected() {
    bool result = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        moveUp();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        moveDown();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        result = true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(75));
    return result;
}

void Menu::addText(sf::Text text) {
    Text.push_back(text);
}
