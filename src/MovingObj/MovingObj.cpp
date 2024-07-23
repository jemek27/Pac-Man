//
// Created by jemek on 10.05.2024.
//

#include "MovingObj/MovingObj.h"
// "{-1, -1} == null"
//todo filename można wywalić
MovingObj::MovingObj(std::pair<int, int> startTileId, int tileSize, const std::string& fileName, char starDir) :
                    TileSize(tileSize),StartTileId(startTileId) , PosTileIDs(startTileId, {-1, -1}),
                    CurrentDirection(starDir), Step(2), Sprite(sf::Sprite()), ImageLoaded(false) {

    if (!texturePng.loadFromFile(fileName)) {
        std::cerr << fileName + " couldn't be read\n";
    } else {
        ImageLoaded = true;
        LastFrame = texturePng.getSize().y - TileSize; // TileSize = OBJSize

        ImagePosition = sf::IntRect(0,0,40,40);
        Sprite = sf::Sprite(texturePng, ImagePosition);
        rotateImageToDir();
        Sprite.setTextureRect(ImagePosition);
    }

    Sprite.setOrigin(tileSize / 2.0, tileSize / 2.0);
    // + tileSize / 2.0 because of the origin
    Sprite.setPosition(tileSize * startTileId.first + tileSize / 2.0, tileSize * startTileId.second + tileSize / 2.0);
}
// -1 because we numerate from 0


void MovingObj::checkTileIds() {
    sf::Vector2 pos = Sprite.getPosition();
    // texture origin is set to the middle
    std::pair<int, int> posInt = {pos.y - TileSize / 2, pos.x - TileSize / 2 };

    if (posInt.first % TileSize != 0) {
        if (posInt.first > PosTileIDs.first_y * TileSize) {
            PosTileIDs.second_y = posInt.first / TileSize + 1;
            PosTileIDs.second_x = posInt.second / TileSize;
        } else {
            PosTileIDs.second_y = posInt.first / TileSize;
            PosTileIDs.second_x = posInt.second / TileSize;
        }
    } else if (posInt.second % TileSize != 0) {
        if (posInt.second > PosTileIDs.first_x * TileSize) {
            PosTileIDs.second_y = posInt.first / TileSize;
            PosTileIDs.second_x = posInt.second / TileSize + 1;
        } else {
            PosTileIDs.second_y = posInt.first / TileSize;
            PosTileIDs.second_x = posInt.second / TileSize;
        }
    } else {
        PosTileIDs.first_y = posInt.first / TileSize;
        PosTileIDs.first_x = posInt.second / TileSize;
        PosTileIDs.second_y = -1;
        PosTileIDs.second_x = -1;
    }
}


void MovingObj::move(float multiplier) {
    if (CurrentDirection == 'N') {
        Sprite.move(0, -Step * multiplier); // up
    }
    else if (CurrentDirection == 'S') {
        Sprite.move(0, Step * multiplier); // down
    }
    else if (CurrentDirection == 'W') {
        Sprite.move(-Step * multiplier, 0); // left
    }
    else if (CurrentDirection == 'E') {
        Sprite.move(Step * multiplier, 0); // right
    }
}


void MovingObj::interact(const std::vector<std::vector<Tile *>> &map, const int &tileSize, sf::Vector2<float> savedPos) {
    move();
    checkTileIds();
    Sprite.setPosition(map[PosTileIDs.second_y][PosTileIDs.second_x]->interact(Sprite.getPosition(), savedPos));
}

void MovingObj::interactBetweenTiles(const std::vector<std::vector<Tile *>> &map, const int &tileSize, sf::Vector2<float> savedPos) {
    move();
    checkTileIds();
    if (PosTileIDs.second_y != -1) {
        Sprite.setPosition(map[PosTileIDs.second_y][PosTileIDs.second_x]->interact(Sprite.getPosition(), savedPos));
    }
}

void MovingObj::setPosTileIDs(std::pair<int, int> firstID, std::pair<int, int> secondID) {
    PosTileIDs = TileIDs(firstID, secondID);
}

void MovingObj::setStartPos() {
    PosTileIDs = {StartTileId, {-1, -1}};
    Sprite.setPosition(TileSize * StartTileId.first + TileSize / 2.0, TileSize * StartTileId.second + TileSize / 2.0);
//    std::cout << StartTileId.first << " " << StartTileId.second << std::endl;
//    std::cout << TileSize * StartTileId.first + TileSize / 2.0 << " " << TileSize * StartTileId.second + TileSize / 2.0 << std::endl;
}

TileIDs MovingObj::getPosTileIDs() {
    return PosTileIDs;
}

void MovingObj::setImagePositionAnimation(const int topPos) {
    ImagePosition.top = topPos;
    Sprite.setTextureRect(ImagePosition);
}

void MovingObj::rotateImageToDir() {
    switch (CurrentDirection){
        case 'N':
            ImagePosition.left = 0;
            break;
        case 'S':
            ImagePosition.left = 40;
            break;
        case 'E':
            ImagePosition.left = 80;
            break;
        case 'W':
            ImagePosition.left = 120;
            break;
    }
}

void MovingObj::shiftFrame() {
    if (ImagePosition.top == LastFrame) {
        ImagePosition.top = 0;
    } else {
        ImagePosition.top += TileSize;
    }
    Sprite.setTextureRect(ImagePosition);
}