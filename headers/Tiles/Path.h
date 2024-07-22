//
// Created by jemek on 18.05.2024.
//

#ifndef Projekt_PATH_H
#define Projekt_PATH_H
#include <iostream>
#include "Tile.h"


class Path : public Tile{
public:
    Path(int tileSize, int x, int y, sf::Sprite& block)
            : Tile(tileSize, x, y, block) {}
    virtual ~Path() = default;
    bool interact() override;
    sf::Vector2<float> interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) override;
};


#endif //SPR2_PATH_H
