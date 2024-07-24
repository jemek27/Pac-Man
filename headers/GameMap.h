//
// Created by jemek on 24.07.2024.
//

#ifndef PAC_MAN_GAMEMAP_H
#define PAC_MAN_GAMEMAP_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <map>

#include "Tiles/Tile.h"
#include "Tiles/Background.h"
#include "Tiles/Path.h"
#include "Tiles/Wall.h"
#include "Tiles/Gate.h"
#include "Tiles/Portal.h"
#include "StationaryObj/StationaryObj.h"
#include "StationaryObj/Point.h"
#include "StationaryObj/Upgrade.h"
#include "StationaryObj/Fruit.h"



class GameMap {
    friend class Game;

    const int TileSize;
    const int FruitValue;

    std::vector<std::string> MapText;
    std::vector<std::vector<Tile*>> Map;
    std::map<std::pair<int, int>,StationaryObj*> StationaryObjs;
    std::vector<std::pair<int,int>> PortalID;
    std::vector<std::pair<int, int>> PathIds;

    std::map<std::string, sf::Texture> MapTextures;
    std::map<std::string, sf::Sprite> MapSprites;
public:
    GameMap(const int tileSize, const int fruitValue);
    ~GameMap();

    void loadMapTextures();
    void createMap();

    void drawMap(sf::RenderWindow* Window);
    void drawStationaryObjs(sf::RenderWindow* Window);
    void drawPortals(sf::RenderWindow* Window);

    void deleteMap();
    void deleteStationaryObjs();

    char checkNeighbourWalls(const int& x, const int& y);
    sf::Sprite wallMatching(const int& x, const int& y);
    sf::Sprite gateMatching(const int& x, const int& y);
};


#endif //PAC_MAN_GAMEMAP_H
