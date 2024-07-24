//
// Created by jemek on 18.05.2024.
//

#include "Tiles/Portal.h"

Portal::Portal(int tileSize, int x, int y, sf::Sprite &block) : Tile(tileSize, x, y, block) {}

Portal::Portal(sf::Vector2<float> pos, int tileSize, int x, int y, sf::Sprite &block)
                : Destination(pos), Tile(tileSize, x, y, block) {
    CenterPosition = sf::Vector2<float>(tileSize * x + tileSize / 2.0, tileSize * y + tileSize / 2.0);
}


bool Portal::interact() {
    return true;
}


sf::Vector2<float> Portal::interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos, float step) {
    if ((pos.x + step == CenterPosition.x and pos.y == CenterPosition.y) or
        (pos.x - step == CenterPosition.x and pos.y == CenterPosition.y) or
        (pos.x == CenterPosition.x and pos.y + step == CenterPosition.y) or
        (pos.x == CenterPosition.x and pos.y - step == CenterPosition.y)   ) {
        return Destination;
    } else {
        return pos;
    }
}