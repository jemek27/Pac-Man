//
// Created by jemek on 03.06.2024.
//

#ifndef PROJEKT_PACMAN_H
#define PROJEKT_PACMAN_H
#include "MovingObj.h"


class PacMan : public MovingObj {
    sf::IntRect ImagePosition;
    bool ImageLoaded;
    char RequestedDirection;
    char StartDir;
public:
    PacMan(std::pair<int, int> startTileId, int tileSize, const std::string& fileName, char starDir);
    void tryMove(const std::vector<std::vector<Tile*>> &map, const int &tileSize);

    bool verticalMovement(char direction);
    bool horizontalMovement(char direction);

    void checkDirection();
    void setResetParams();
    void rotateImageToDir();
};


#endif //PROJEKT_PACMAN_H
