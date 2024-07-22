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
    char Fps;
    char FrameCounter;
    char AnimationCount;
    const char TextureShiftFrameThreshold;
    const char BlinkCouterThreshold;

    int TileSize;
    std::vector<std::string> MapText;
    std::vector<std::vector<Tile*>> Map;
    std::map<std::pair<int, int>,StationaryObj*> StationaryObjs;
    std::vector<std::pair<int,int>> PortalID;
    std::vector<std::pair<int, int>> PathIds;

    char Lives;
    PacMan* PacMan;
    bool UpgradeOn;
    int UpgradeTimer;
    int UpgradeBlinkCouter;
    int StartNumOfPoints;
    bool FruitNotYetAppeared;

    sf::Texture GhostEatenUpgradeTexturePng;
    sf::IntRect ImagePosition;
    sf::Sprite UpgradeGhost;
//    sf::RenderTexture GhostTextureEaten;
//    sf::RenderTexture GhostTextureUpgrade;
    std::vector<Ghost*> Ghosts;
    char EatenCount;

    NumberDisplay* ScoreDisplay;
    int Score;
    NumberDisplay* LivesDisplay;

    MainMenu* TheMainMenu;
    PauseMenu* ThePauseMenu;

    std::map<std::string, sf::Texture> MapTextures;
    std::map<std::string, sf::Sprite> MapSprites;
public:
    Game(int WindowH, int WindowW, int fps, const std::string& name);
    ~Game();
    void deleteMap();
    void deleteStationaryObjs();

    void initiateGamePlay();
    bool getInGameplay() const;
    void loadMapTextures();
    void loadTexture();

    void actInMainMenu();
    void drawMainMenu();
    void drawPauseMenu();

    void createMap();
    char checkNeighbourWalls(const int& x, const int& y);
    sf::Sprite wallMatching(const int& x, const int& y);
    sf::Sprite gateMatching(const int& x, const int& y);
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
    void checkUpgradeTime();
    void upgradeOff();
    void checkTextureShift();
    void textureShift(const int topPos);
    void checkFruitAppearance();
    void ghostInteractions();
};


#endif //SPR2_GAME_H
