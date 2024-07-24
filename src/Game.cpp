//
// Created by jemek on 10.05.2024.
//

#include "Game.h"
//todo osobna klasa mapy?
//todo animacja przy skuciu się
//todo pod koniec portalu powinno nastąpić przejście
Game::Game(int windowH, int windowW, int fps, const std::string& name)
            : TileSize(40), LabiryntMap(GameMap(TileSize, 200)), VideoMode(windowH, windowW), Fps(fps), FrameCounter(0),
              PacmanTextureShiftFrameThreshold(Fps / 15), BlinkCouterThreshold(Fps / 10), ScoreDisplay(nullptr),
              AnimationCount(0), LivesDisplay(nullptr), PacMan(nullptr), Score(0), UpgradeOn(false), StartNumOfPoints(0),
              FruitAppeared(false), ConstFruitTimeCounter(Fps * 10), UpgradeTimer(0), EatenCount(0), Lives(3),
              UpgradeBlinkCouter(0), PacmanTextureShiftCounter(0), GhostTextureShiftThreshold(3) {

    Window = new sf::RenderWindow(VideoMode, name);
    Window->setFramerateLimit(fps);

    if (!Font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Failed to load Font file!" << std::endl;
    }

    LabiryntMap.loadMapTextures();

    TheMainMenu = new MainMenu(Window, Font);
    ThePauseMenu = new PauseMenu(Window, Font);

    std::cout << TileSize << "\n";
    loadTexture();
    initiateGamePlay();
}

Game::~Game() {
    delete PacMan;
    delete ScoreDisplay;
    delete TheMainMenu;
    delete ThePauseMenu;
    for (auto& ghost : Ghosts) {
        delete ghost;
    }
    delete Window;
}



void Game::loadTexture() {
    if (!GhostEatenUpgradeTexturePng.loadFromFile("assets/upgradeEatenGhostAnimated_80x80.png")) {
        std::cerr << "assets/upgradeEatenGhostAnimated_80x80.png not loaded" << std::endl;
    } else {
        ImagePosition = sf::IntRect(0,0,40,40);
        UpgradeGhost = sf::Sprite(GhostEatenUpgradeTexturePng, ImagePosition);
        UpgradeGhost.setTextureRect(ImagePosition);
        UpgradeGhost.setOrigin(TileSize / 2.0, TileSize / 2.0);
    }
}

//todo raz wczytać tekstury do programu (cha trzeba od razu Sprite zrobić)
void Game::initiateGamePlay() {
    if (!LabiryntMap.Map.empty()) { LabiryntMap.deleteMap(); } //todo to w metoda
    if (!LabiryntMap.StationaryObjs.empty()) { LabiryntMap.deleteStationaryObjs(); }
    LabiryntMap.createMap();

    if (PacMan != nullptr) {
        PacMan->setPosTileIDs({12, 19}, {-1, -1});
        PacMan->setResetParams();
    } else {
        PacMan = new class PacMan({12, 19}, TileSize, "assets/pacmanAnimated160x240.png", 'N');
    }

    if (Ghosts.empty()) {

        Ghosts.push_back(new Ghost({12, 10}, TileSize, "assets/redGhostAnimated_80x160.png", 'N', &LabiryntMap.PathIds,
                                   &LabiryntMap.MapText));
        Ghosts.push_back(new Ghost({12, 9}, TileSize, "assets/greenGhostAnimated_80x160.png", 'N', &LabiryntMap.PathIds,
                                   &LabiryntMap.MapText));
        Ghosts.push_back(new Ghost({12, 11}, TileSize, "assets/cyanGhostAnimated_80x160.png", 'N', &LabiryntMap.PathIds,
                                   &LabiryntMap.MapText));
        Ghosts.push_back(new Ghost({14, 10}, TileSize, "assets/magentaGhostAnimated_80x160.png", 'N', &LabiryntMap.PathIds,
                                   &LabiryntMap.MapText));

    } else {
        for (auto &ghost: Ghosts) { ghost->setResetParams(); }
    }

    ThePauseMenu->InGameplay = true;

    if (ScoreDisplay == nullptr) {
        ScoreDisplay = new NumberDisplay(Font, TileSize / 4 * 3, {TileSize * 3, TileSize / 5 * 2});
    }
    if (LivesDisplay == nullptr) {
        LivesDisplay = new NumberDisplay(Font, TileSize / 4 * 3, {TileSize * 3, TileSize * 23});
    }

    UpgradeOn = false;
    UpgradeTimer = 0;
    EatenCount = 0;
    Score = 0;
    Lives = 3;

    auto visited = Ghosts[0]->checkVisited(LabiryntMap.MapText);
    for (auto &g: Ghosts) {
        g->Visited = visited;
        g->selectRandomTarget();
        g->findRoute();
    }

    FruitAppeared = false;
    FruitTimeCounter = ConstFruitTimeCounter;
    StartNumOfPoints = LabiryntMap.StationaryObjs.size();
}

void Game::update() {
    pullEvent();

    if (!ThePauseMenu->GamePaused) {
        ThePauseMenu->checkPause();

        if (!ThePauseMenu->InGameplay) {
            TheMainMenu->InGameplay = ThePauseMenu->InGameplay;
        }

        if (!ThePauseMenu->GamePaused) {
            checkUpgradeTime();
            checkAndShiftTexture();
            PacMan->tryMove(LabiryntMap.Map, TileSize);

            //std::thread thStationaryObjsCollision(&Game::checkStationaryObjsCollision, this);

            //std::vector<std::future<void>> futures;
            //for (const auto& ghost : Ghosts) {
            //    futures.push_back(std::async(std::launch::async, &Ghost::checkAndMove, ghost));
            //}

            for (const auto& ghost : Ghosts) {
                ghost->checkAndMove();
            }
            checkStationaryObjsCollision();

            //for (auto& f : futures) { f.wait(); }
            //thStationaryObjsCollision.join();
            checkFruitAppearance();

            ghostInteractions();

            ScoreDisplay->text.setString("1UP\n" + std::to_string(Score));
            LivesDisplay->text.setString("LIVE'S: " + std::to_string(Lives));

            if (LabiryntMap.StationaryObjs.empty()) {
                gameplayHasEnded();
            }
        }
    } else {
        ThePauseMenu->pauseScreen();
    }
}

void Game::render() {
    if (TheMainMenu->InGameplay) {
        Window->clear(sf::Color::Black);

        LabiryntMap.drawMap(Window);
        LabiryntMap.drawStationaryObjs(Window);

        Window->draw(PacMan->Sprite);

        drawGhosts();

        Window->draw(ScoreDisplay->text);
        Window->draw(LivesDisplay->text);
        LabiryntMap.drawPortals(Window);

        if (ThePauseMenu->GamePaused) { drawPauseMenu(); }


        // this is a minimal sleep to ensure some
        // measurable time passes even on very fast machines
        sf::sleep(sf::microseconds(1));

        Window->display();
    }
}

bool Game::running() {
    return Window->isOpen();
}

bool Game::checkCollision(const sf::Vector2<float>& pos1, const int& size1,
                          const sf::Vector2<float>& pos2, const int& size2) {
    bool result = false;
    int halfS1 = size1/2;
    int halfS2 = size2/2;

    if (pos1.x == pos2.x) {
        if (pos1.y > pos2.y) { //obj1 is under obj2
            if (pos1.y - halfS1 < pos2.y + halfS2) {
                result = true;
            }
        }
        else if (pos1.y < pos2.y)  {
            if (pos2.y - halfS2 < pos1.y + halfS1) {
                result = true;
            }
        }
        else {
            result = true;
        }
    }
    else if (pos1.y == pos2.y) {
        if (pos1.x > pos2.x) { //obj1 is under obj2
            if (pos1.x - halfS1 < pos2.x + halfS2) {
                result = true;
            }
        }
        else {
            if (pos2.x - halfS2 < pos1.x + halfS1) {
                result = true;
            }
        }
    }

    return result;
}

bool Game::getInGameplay() const {
    return TheMainMenu->InGameplay;
}

void Game::actInMainMenu() {
    pullEvent();
    TheMainMenu->interact();
}

void Game::drawMainMenu() {
    Window->clear(sf::Color::Black);
    TheMainMenu->draw(*Window);
    Window->display();
}

void Game::pullEvent() {
    while (Window->pollEvent(Event))
    {
        if (Event.type == sf::Event::Closed) {
            Window->close();
            TheMainMenu->InGameplay = false;
        }
    }
}

void Game::checkStationaryObjsCollision() {
    auto TileIDs = PacMan->getPosTileIDs();
    if (LabiryntMap.StationaryObjs.contains({TileIDs.second_x, TileIDs.second_y})) {
        std::vector<std::pair<int, int>> ids = {{TileIDs.second_x, TileIDs.second_y}};

        if (LabiryntMap.StationaryObjs.contains({TileIDs.first_x, TileIDs.first_y})) {
            ids.push_back({TileIDs.first_x, TileIDs.first_y});
        }

        for (auto id: ids) {
            if (checkCollision(PacMan->Sprite.getPosition(), TileSize, LabiryntMap.StationaryObjs[id]->getPosition(),
                               LabiryntMap.StationaryObjs[id]->getSize())) {
                std::pair<int, bool> scoreAndIsUpgrade = LabiryntMap.StationaryObjs[id]->interact();

                delete LabiryntMap.StationaryObjs[id];
                LabiryntMap.StationaryObjs.erase(id);

                Score += scoreAndIsUpgrade.first;
                if (scoreAndIsUpgrade.second) {
                    UpgradeOn = true;
                    UpgradeTimer = Fps * 9;
                    UpgradeBlinkCouter = Fps / 5;
                    for (auto &g: Ghosts) {
                        g->requestSpeedChange(0.5f);
                    }
                }
            }
        }
    }
}

void Game::drawPauseMenu() {
    auto size = Window->getSize();
    sf::RectangleShape rectangle(sf::Vector2f(size.x, size.y));
    rectangle.setFillColor(sf::Color(0, 0, 0, 128)); // 128 to 50% przezroczystości
    Window->draw(rectangle);
    ThePauseMenu->draw(*Window);
}

void Game::drawGhosts() {
    if (UpgradeOn) {
        bool statBlinking = false;
        if (UpgradeTimer < Fps * 2) {
            statBlinking = true;
            --UpgradeBlinkCouter;
        }
        for (const auto& ghost : Ghosts) {
            if (ghost->Eaten) {
                ImagePosition.left = 40;
                UpgradeGhost.setTextureRect(ImagePosition);
                UpgradeGhost.setPosition(ghost->Sprite.getPosition());
                Window->draw(UpgradeGhost);
            } else if (!ghost->EatenAtCurrUpgrade and !ghost->Eaten) {
                if (statBlinking) {
                    if (UpgradeBlinkCouter < BlinkCouterThreshold) {
                        ImagePosition.left = 0;
                        UpgradeGhost.setTextureRect(ImagePosition);
                        UpgradeGhost.setPosition(ghost->Sprite.getPosition());
                        Window->draw(UpgradeGhost);
                        if (UpgradeBlinkCouter == 0) { UpgradeBlinkCouter = Fps / 5; }
                    }
                } else {
                    ImagePosition.left = 0;
                    UpgradeGhost.setTextureRect(ImagePosition);
                    UpgradeGhost.setPosition(ghost->Sprite.getPosition());
                    Window->draw(UpgradeGhost);
                }
            } else {
                Window->draw(ghost->Sprite);
            }
        }
    } else {
        for (const auto& ghost : Ghosts) {
            if (ghost->Eaten) {
                ImagePosition.left = 40;
                UpgradeGhost.setTextureRect(ImagePosition);
                UpgradeGhost.setPosition(ghost->Sprite.getPosition());
                Window->draw(UpgradeGhost);
            } else {
                Window->draw(ghost->Sprite);
            }
        }
    }

}

void Game::checkUpgradeTime() {
    if (UpgradeOn) {
        if (UpgradeTimer > 0) { --UpgradeTimer; }
        else { upgradeOff(); }
    }
}

void Game::gameplayHasEnded() {
    TheMainMenu->InGameplay = false;
    TheMainMenu->manageNewScore(Score);
    TheMainMenu->viewEndScore(Score);
    TheMainMenu->viewScoreboard();
}

void Game::setStartPos() {
    PacMan->setResetParams();
    for (auto& ghost : Ghosts) {
        ghost->setResetParams() ;
    }
}

void Game::upgradeOff() {
    UpgradeOn = false;
    EatenCount = 0;
    for (auto& g : Ghosts) {
        if (!g->Eaten) {
            g->requestSpeedChange(1.0f);
        }
        g->EatenAtCurrUpgrade = false;
    }
}

void Game::checkFruitAppearance() {
    if (!FruitAppeared and StartNumOfPoints - 100 >= LabiryntMap.StationaryObjs.size()) {
        FruitAppeared = true;

        sf::Vector2<float> position(TileSize * 12 + TileSize / 2.0, TileSize * 13 + TileSize / 2.0);
        if (LabiryntMap.StationaryObjs.contains({12, 13})) { // if a point is there
            delete LabiryntMap.StationaryObjs[{12, 13}];
            LabiryntMap.StationaryObjs[{12, 13}] = new Fruit(20, position, 250);
        } else {
            LabiryntMap.StationaryObjs[{12, 13}] = new Fruit(20, position, 200);
        }

    }

    if (FruitAppeared and FruitTimeCounter != 0) {
        if (--FruitTimeCounter == 0) {
            if (LabiryntMap.StationaryObjs[{12, 13}]->interact().first == LabiryntMap.FruitValue) {
                delete LabiryntMap.StationaryObjs[{12, 13}];
            } else {
                delete LabiryntMap.StationaryObjs[{12, 13}];
                LabiryntMap.StationaryObjs[{12, 13}] = new Point(4, sf::Vector2<float>(TileSize * 12 + TileSize / 2.0,
                                                                                        TileSize * 13 + TileSize / 2.0));
            }
        }
    }
}

void Game::ghostInteractions() {
    for (const auto& ghost : Ghosts) {
        if (checkCollision(PacMan->Sprite.getPosition(), TileSize, ghost->Sprite.getPosition(), TileSize) and
            !ghost->Eaten) {

            if (UpgradeOn and !ghost->EatenAtCurrUpgrade) {
                auto tempUsunpotem = Score;
                Score += 100 << ++EatenCount;
                std::cout << Score - tempUsunpotem << std::endl;
                ghost->EatenAtCurrUpgrade = true;
                std::cout << "colision with ghost" << std::endl;
                ghost->wasEaten();
            } else {
                render();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                if (--Lives == 0) { gameplayHasEnded(); }
                else {
                    setStartPos();
                    upgradeOff();
                }
            }
        }
    }
}

void Game::checkAndShiftTexture() {
    if (++FrameCounter > PacmanTextureShiftFrameThreshold) {
        FrameCounter = 0;
        ++PacmanTextureShiftCounter;
        PacMan->shiftFrame();
    }

    if (PacmanTextureShiftCounter == GhostTextureShiftThreshold) {
        PacmanTextureShiftCounter = 0;
        
        for (auto g : Ghosts) { g->shiftFrame(); }
        PacMan->shiftFrame();

        if (AnimationCount++ == 0) { //only to animation frames
            ImagePosition.top = 40;
        } else {
            ImagePosition.top = 0;
            AnimationCount = 0;
        }
        UpgradeGhost.setTextureRect(ImagePosition);
    }
}