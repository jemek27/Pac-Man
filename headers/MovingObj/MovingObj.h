#ifndef Projekt_MOVINGOBJ_H
#define Projekt_MOVINGOBJ_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Tiles/Tile.h"

//[i][j] i -> y j -> x
struct TileIDs {
    int first_y = -1; //-1 because there is no ID < 0
    int first_x = -1;
    int second_y = -1;
    int second_x = -1;
    TileIDs(std::pair<int, int> firstID, std::pair<int, int> secondID)
            : first_y(firstID.first), first_x(firstID.second), second_y(secondID.first), second_x(secondID.second) {}
};


class MovingObj {
protected:
    sf::RenderTexture Texture;
    sf::Texture texturePng;
    sf::IntRect ImagePosition;
    unsigned int LastFrame;
    bool AnimationForward;
    bool ImageLoaded;

    TileIDs PosTileIDs;
    std::pair<int, int> StartTileId;
    int TileSize;
    char CurrentDirection; // N S E W
    float Step;
public:
    sf::Sprite Sprite;

    MovingObj(std::pair<int, int> startTileId, int tileSize, const std::string& fileName, char starDir);

    void setPosTileIDs(std::pair<int, int> firstID, std::pair<int, int> secondID);
    void checkTileIds();

    void interact(const std::vector<std::vector<Tile*>> &map, const int &tileSize, sf::Vector2<float> savedPos);
    void interactBetweenTiles(const std::vector<std::vector<Tile *>> &map, const int &tileSize, sf::Vector2<float> savedPos);

    void move(float multiplier = 1);
    void setStartPos();
    TileIDs getPosTileIDs();
    void setImagePositionAnimation(const int topPos);
    void rotateImageToDir();
    void shiftFrame();
};




#endif //SPR2_MOVINGOBJ_H
