//
// Created by jemek on 18.05.2024.
//

#include "Tiles/Wall.h"

bool Wall::interact() {
    std::cout << " Wall in way ";
    return false;
}

sf::Vector2<float> Wall::interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) {
    return savedPos;
}
