//
// Created by jemek on 18.05.2024.
//

#ifndef Projekt_PORTAL_H
#define Projekt_PORTAL_H
#include <iostream>
#include "Tile.h"


class Portal : public Tile {
    sf::Vector2<float> Destination;
public:
    Portal(int tileSize, int x, int y)
            : Tile(tileSize, x, y, sf::Color::Black) {}

    Portal(sf::Vector2<float> pos, int tileSize, int x, int y) : Destination(pos), Tile(tileSize, x, y, sf::Color::Black) {}
    virtual ~Portal() = default;

    bool interact() override;
    sf::Vector2<float> interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) override;
};


#endif
