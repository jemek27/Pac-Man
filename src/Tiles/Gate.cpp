//
// Created by jemek on 18.05.2024.
//

#include "Tiles/Gate.h"

bool Gate::interact() {
    std::cout << " Gate in way ";
    return false;
}


sf::Vector2<float> Gate::interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos, float step) {
    return savedPos;
}


