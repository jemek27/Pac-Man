//
// Created by jemek on 03.06.2024.
//

#include "MovingObj/PacMan.h"


PacMan::PacMan(std::pair<int, int> startTileId, int tileSize, const std::string &fileName, char starDir) : MovingObj(
        startTileId, tileSize, fileName, starDir), RequestedDirection(starDir), StartDir(starDir) {



    if (!texturePng.loadFromFile(fileName)) {
        ImageLoaded = false;
        std::cerr << fileName + " couldn't be read\n";
        // Creating a replacement Texture
        Sprite = sf::Sprite();
        sf::CircleShape circle(tileSize / 2.0);
        circle.setFillColor(sf::Color::Yellow);

        Texture.create(tileSize, tileSize);
        Texture.clear(sf::Color::Transparent);
        Texture.draw(circle);
        Texture.display();

        Sprite.setTexture(Texture.getTexture());

    } else {
        ImageLoaded = true;
        ImagePosition = sf::IntRect(0,0,40,40);
        Sprite = sf::Sprite(texturePng, ImagePosition);
        rotateImageToDir();
        Sprite.setTextureRect(ImagePosition);
    }

    Sprite.setOrigin(tileSize / 2.0, tileSize / 2.0);
    // + tileSize / 2.0 because of the origin
    Sprite.setPosition(tileSize * startTileId.first + tileSize / 2.0, tileSize * startTileId.second + tileSize / 2.0);
}


void PacMan::tryMove(const std::vector<std::vector<Tile*>> &map, const int &tileSize) {
    auto saveOfPos = Sprite.getPosition();
    auto saveCurDir = CurrentDirection;
    bool changeDir = false;
    checkDirection();
    checkTileIds(tileSize);

    if (CurrentDirection != RequestedDirection) { changeDir = true; }

    if (PosTileIDs.second_y == -1) { //is at the crossroads

        if (changeDir) { CurrentDirection = RequestedDirection; }

        interact(map, tileSize, saveOfPos);
        if (Sprite.getPosition() == saveOfPos) CurrentDirection = saveCurDir;

            if (changeDir) { //if the direction was changed, we need to try again with the old one

                interact(map, tileSize, saveOfPos);
                if (Sprite.getPosition() == saveOfPos) CurrentDirection = saveCurDir;
            }

    } else {
        if (changeDir) {
            if ((verticalMovement(RequestedDirection) and verticalMovement(CurrentDirection)) or
                (horizontalMovement(RequestedDirection) and horizontalMovement(CurrentDirection)) ) {
                CurrentDirection = RequestedDirection;
                TileIDs temp({PosTileIDs.second_y, PosTileIDs.second_x}, {PosTileIDs.first_y, PosTileIDs.first_x});
                PosTileIDs = temp;
            }
        }

        interactBetweenTiles(map, tileSize, saveOfPos);
    }
    if (ImageLoaded) {
        rotateImageToDir();
        Sprite.setTextureRect(ImagePosition);
    }
}


bool PacMan::verticalMovement(char direction) {
    return direction == 'N' or direction == 'S';
}

bool PacMan::horizontalMovement(char direction) {
    return direction == 'W' or direction == 'E';
}


void PacMan::checkDirection() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        RequestedDirection = 'N';
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        RequestedDirection = 'S';
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        RequestedDirection = 'W';
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        RequestedDirection = 'E';
    }
}

void PacMan::setResetParams() {
    setStartPos();
    CurrentDirection = StartDir;
    RequestedDirection = StartDir;
}

void PacMan::rotateImageToDir() {
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

