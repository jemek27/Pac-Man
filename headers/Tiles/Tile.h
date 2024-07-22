//
// Created by jemek on 18.05.2024.
//

#ifndef Projekt_TILE_H
#define Projekt_TILE_H
#include <SFML/Graphics.hpp>

class Tile {
protected:
    sf::Sprite Block;
public:
    Tile(int tileSize, int x, int y, sf::Sprite& block);
    virtual ~Tile() = default;
    virtual bool interact() = 0;
    virtual sf::Vector2<float> interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) = 0;

    virtual sf::Sprite getBlock();
};


#endif
