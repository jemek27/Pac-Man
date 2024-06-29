//
// Created by jemek on 07.06.2024.
//

#include "StationaryObj/StationaryObj.h"

StationaryObj::StationaryObj(const int& size, const sf::Vector2<float>& position, const int& scoreNumber)
                            : Size(size), Position(position), ScoreNumber(scoreNumber) {}

sf::Vector2<float> StationaryObj::getPosition() {
    return Position;
}

int StationaryObj::getSize() {
    return Size;
}

sf::Sprite StationaryObj::getSprite() {
    return Sprite;
}
