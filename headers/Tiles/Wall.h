//
// Created by jemek on 18.05.2024.
//

#ifndef Projekt_WALL_H
#define Projekt_WALL_H
#include <iostream>
#include "Tile.h"


class Wall : public Tile {
public:
    Wall(int tileSize, int x, int y)
            : Tile(tileSize, x, y, sf::Color::Blue) {}
    virtual ~Wall() = default;
    bool interact() override;
    sf::Vector2<float> interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) override;
};


#endif
