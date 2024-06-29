//
// Created by jemek on 03.06.2024.
//

#include "Tiles/Background.h"

bool Background::interact() {
    std::cout << " Gate in way ";
    return false;
}


sf::Vector2<float> Background::interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) {
    return savedPos;
}
