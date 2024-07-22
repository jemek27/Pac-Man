//
// Created by jemek on 18.05.2024.
//

#include "Tiles/Tile.h"

Tile::Tile(int tileSize, int x, int y, sf::Sprite& block)
        : Block(block) {
    Block.setPosition(x * tileSize + 20, y * tileSize + 20);
    Block.setOrigin(20, 20);
}

sf::Sprite Tile::getBlock() {
    return Block;
}