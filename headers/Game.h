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

#include "GameMap.h"
#include "MovingObj/MovingObj.h"
#include "MovingObj/PacMan.h"
#include "MovingObj/Ghost.h"
#include "NumberDisplay.h"
#include "Menu/MainMenu.h"
#include "Menu/PauseMenu.h"



class Game {
    sf::RenderWindow* Window;
    sf::Event Event;
    sf::VideoMode VideoMode;
    sf::Font Font;
    char Fps;
    char FrameCounter;
    char AnimationCount;
    char PacmanTextureShiftCounter;
    const char PacmanTextureShiftFrameThreshold;
    const char GhostTextureShiftThreshold;
    const char BlinkCouterThreshold;

    const int TileSize;
    GameMap LabiryntMap;

    char Lives;
    PacMan* PacMan;
    bool UpgradeOn;
    int UpgradeTimer;
    int UpgradeBlinkCouter;
    int StartNumOfPoints;
    bool FruitAppeared;
    const int ConstFruitTimeCounter;
    int FruitTimeCounter;


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
public:
    Game(int WindowH, int WindowW, int fps, const std::string& name);
    ~Game();

    void initiateGamePlay();
    bool getInGameplay() const;
    void loadTexture();

    void actInMainMenu();
    void drawMainMenu();
    void drawPauseMenu();

    bool running();
    void pullEvent();
    void update();
    void render();

    void drawGhosts();

    void gameplayHasEnded();
    void setStartPos();

    bool checkCollision (const sf::Vector2<float>& pos1, const int& size1,
                         const sf::Vector2<float>& pos2, const int& size2);
    void checkStationaryObjsCollision();
    void checkUpgradeTime();
    void upgradeOff();
    void checkAndShiftTexture();
    void checkFruitAppearance();
    void ghostInteractions();
};


#endif //SPR2_GAME_H
