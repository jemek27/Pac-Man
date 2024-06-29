//
// Created by jemek on 18.05.2024.
//

#ifndef Projekt_GATE_H
#define Projekt_GATE_H
#include <iostream>
#include "Tile.h"


class Gate : public Tile {
public:
    Gate(int tileSize, int x, int y)
            : Tile(tileSize, x, y, sf::Color::Yellow) {}

    virtual ~Gate() = default;
    bool interact() override;
    sf::Vector2<float> interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) override;
};


#endif //SPR2_GATE_H
