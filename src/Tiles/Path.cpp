//
// Created by jemek on 18.05.2024.
//

#include "Tiles/Path.h"

bool Path::interact() {
    std::cout << " GO! ";
    return true;
}

sf::Vector2<float> Path::interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) {
    return pos;
}

