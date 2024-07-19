//
// Created by jemek on 10.05.2024.
//

#include "MovingObj/MovingObj.h"
// "{-1, -1} == null"
//todo filename można wywalić
MovingObj::MovingObj(std::pair<int, int> startTileId, int tileSize, const std::string& fileName, char starDir) :
                    TileSize(tileSize),StartTileId(startTileId) , PosTileIDs(startTileId, {-1, -1}),
                    CurrentDirection(starDir), Step(2) {}
// -1 because we numerate from 0


void MovingObj::checkTileIds(const int &tileSize) {
    sf::Vector2 pos = Sprite.getPosition();
    // texture origin is set to the middle
    std::pair<int, int> posInt = {pos.y - tileSize / 2, pos.x - tileSize / 2 };

    if (posInt.first % tileSize != 0) {


        if (posInt.first > PosTileIDs.first_y * tileSize) {
            PosTileIDs.second_y = posInt.first / tileSize + 1;
            PosTileIDs.second_x = posInt.second / tileSize;
        } else {
            PosTileIDs.second_y = posInt.first / tileSize;
            PosTileIDs.second_x = posInt.second / tileSize;
        }

//        std::cout << "~~TWO Blocks id1:"  << PosTileIds.first.first << " " << PosTileIds.first.second << "   id2:"
//                  << PosTileIds.second.first << " " << PosTileIds.second.second << "   ";

    } else if (posInt.second % tileSize != 0) {

        if (posInt.second > PosTileIDs.first_x * tileSize) {
            PosTileIDs.second_y = posInt.first / tileSize;
            PosTileIDs.second_x = posInt.second / tileSize + 1;
        } else {
            PosTileIDs.second_y = posInt.first / tileSize;
            PosTileIDs.second_x = posInt.second / tileSize;
        }

//        std::cout << "TWO Blocks id1:"  << PosTileIds.first.first << " " << PosTileIds.first.second << "   id2:"
//                    << PosTileIds.second.first << " " << PosTileIds.second.second << "   ";

    } else {
        PosTileIDs.first_y = posInt.first / tileSize;
        PosTileIDs.first_x = posInt.second / tileSize;
        PosTileIDs.second_y = -1;
        PosTileIDs.second_x = -1;

//        std::cout << "ONE block " << PosTileIds.first.first << " " << PosTileIds.first.second << "   ";
    }

//    std::cout << pos.x << "; " << pos.y << "\n";
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
    checkTileIds(tileSize);
    Sprite.setPosition(map[PosTileIDs.second_y][PosTileIDs.second_x]->interact(Sprite.getPosition(), savedPos));
}

void MovingObj::interactBetweenTiles(const std::vector<std::vector<Tile *>> &map, const int &tileSize, sf::Vector2<float> savedPos) {
    move();
    checkTileIds(tileSize);
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



