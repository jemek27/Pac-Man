//
// Created by jemek on 18.05.2024.
//

#ifndef Projekt_TILE_H
#define Projekt_TILE_H
#include <SFML/Graphics.hpp>

class Tile {
protected:
    int TileSize;
    int x;
    int y;
    sf::RectangleShape* block;
public:
    Tile(int tileSize, int x, int y, sf::Color color)
            : TileSize(tileSize), x(x), y(y), block(new sf::RectangleShape(sf::Vector2f(tileSize, tileSize))) {
        block->setPosition(x * tileSize, y * tileSize);
        block->setFillColor(color);
    }
    virtual ~Tile() = default;
    virtual bool interact() = 0;
    virtual sf::Vector2<float> interact(sf::Vector2<float> pos, sf::Vector2<float> savedPos) = 0;

    virtual sf::RectangleShape getBlock();
};


#endif
