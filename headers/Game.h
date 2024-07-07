#ifndef Projekt_GAME_H
#define Projekt_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <chrono>
#include <functional>


#include <thread>
#include <regex>
#include <future>

#include "MovingObj/MovingObj.h"
#include "MovingObj/PacMan.h"
#include "MovingObj/Ghost.h"
#include "NumberDisplay.h"
#include "Menu/MainMenu.h"
#include "Menu/PauseMenu.h"
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



class Game {
    sf::RenderWindow* Window;
    sf::Event Event;
    sf::VideoMode VideoMode;
    sf::Font Font;
    int Fps;

    int TileSize;
    std::vector<std::string> MapText;
    std::vector<std::vector<Tile*>> Map;
    std::map<std::pair<int, int>,StationaryObj*> StationaryObjsM;
    std::vector<std::pair<int,int>> PortalID;
    std::vector<std::pair<int, int>> PathIds;

    char Lifes;
    PacMan* PacMan;
    bool UpgradeOn;
    int UpgradeTimer;
    int UpgradeBlinkCouter;
    int StartNumOfPoints;
    bool FruitNotYetAppeared;

    sf::RenderTexture GhostTextureEaten;
    sf::RenderTexture GhostTextureUpgrade;
    std::vector<Ghost*> Ghosts;
    char EatenCount;

    NumberDisplay* ScoreDisplay;
    int Score;
    NumberDisplay* LifesDisplay;


    MainMenu* TheMainMenu;
    PauseMenu* ThePauseMenu;
public:
    Game(int WindowH, int WindowW, int fps, const std::string& name);
    ~Game();
    void deleteMap();
    void deleteStationaryObjs();

    void initiateGamePlay();
    bool getInGameplay() const;
    void loadTexture();

    void actInMainMenu();
    void drawMainMenu();
    void drawPauseMenu();

    void createMap();
    bool running();
    void pullEvent();
    void update();
    void render();
    void drawMap();
    void drawStationaryObjs();
    void drawPortals();
    void drawGhosts();

    void gameplayHasEnded();
    void setStartPos();

    bool checkCollision (const sf::Vector2<float>& pos1, const int& size1,
                         const sf::Vector2<float>& pos2, const int& size2);
    void checkStationaryObjsCollision();
    void checkStationaryObjsCollision_v2();
    void checkUpgradeTime();
    void upgradeOff();
    void checkFruitAppearance();
    void ghostInteractions();
};


#endif //SPR2_GAME_H
