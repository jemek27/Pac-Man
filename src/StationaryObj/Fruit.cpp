//
// Created by jemek on 07.06.2024.
//

#include "StationaryObj/Fruit.h"

Fruit::Fruit(const int& size, const sf::Vector2<float>& position, int points) : StationaryObj(size, position, points){
    Sprite = sf::Sprite();

    sf::CircleShape circle(Size / 2.0);
    circle.setFillColor(sf::Color::Red);

    Texture.create(Size, Size);
    Texture.clear(sf::Color::Transparent);
    Texture.draw(circle);
    Texture.display();

    Sprite.setTexture(Texture.getTexture());

    Sprite.setOrigin(Size / 2.0, Size/ 2.0);
    Sprite.setPosition(position.x,position.y);
}

std::pair<int, bool> Fruit::interact() {
    return {ScoreNumber, false}; //bool -> is upgrade
}