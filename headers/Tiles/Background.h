//
// Created by jemek on 03.06.2024.
//

#ifndef PROJEKT_BACKGROUND_H
#define PROJEKT_BACKGROUND_H

#include <iostream>
#include "Tiles/Tile.h"


class Background : public Tile {
public:
    Background(int tileSize, int x, int y, sf::Sprite& block)
            : Tile(tileSize, x, y, block) {}

    virtual ~Background() = default;
    bool interact() override;
    sf::Vector2<float> interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) override;
};


#endif //PROJEKT_BACKGROUND_H
