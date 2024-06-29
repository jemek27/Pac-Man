//
// Created by jemek on 10.05.2024.
//

#include "Game.h"

Game::Game(int windowH, int windowW, int fps, const std::string& name)
            : VideoMode(windowH, windowW), Fps(fps), ScoreDisplay(nullptr), LifesDisplay(nullptr), PacMan(nullptr),
              Score(0), UpgradeOn(false), StartNumOfPoints(0), FruitNotYetAppeared(true), UpgradeTimer(0),
              EatenCount(0), Lifes(3), UpgradeBlinkCouter(0) {

    Window = new sf::RenderWindow(VideoMode, name);
    Window->setFramerateLimit(fps);

    if (!Font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Failed to load Font file!" << std::endl;
    }

    TheMainMenu = new MainMenu(Window, Font);
    ThePauseMenu = new PauseMenu(Window, Font);

    MapText = {
            "BBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBB###################BBB",
            "BBB#.....#...........#BBB",
            "BBB#.#*#.#.###.#.#*#.#BBB",
            "BBB#.###.....#.#.....#BBB",
            "BBB#.#.#.###.#.#.###.#BBB",
            "BBB#...#.#.....#.....#BBB",
            "BBB#.###.#.###.#####.#BBB",
            "BBB#.......# #.......#BBB",
            "BBB###.###.# $.#.#####BBB",
            "BBBBB#.#B#.# #.#.#BBBBBBB",
            "BBB###.###.###.#.#####BBB",
            "BBP....#.......#......PBB",
            "BBB###.#.###.#.###.###BBB",
            "BBBBB#...#...#.....#BBBBB",
            "BBB#####.###.#.#######BBB",
            "BBB#.......#.#.......#BBB",
            "BBB#.###.###.###.###.#BBB",
            "BBB#.............#...#BBB",
            "BBB###.#.#####.#.#.###BBB",
            "BBB#.*.#...........*.#BBB",
            "BBB###################BBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBB"
    };

    TileSize = std::min(windowW / MapText[0].size(), windowH / MapText.size());
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
    deleteMap();
    deleteStationaryObjs();
    delete Window;
}

void Game::initiateGamePlay() {
    if (!Map.empty()) { deleteMap(); }
    if (!StationaryObjs.empty()) { deleteStationaryObjs(); }
    if (PacMan != nullptr) {
        PacMan->setPosTileIDs({12, 19}, {-1, -1});
        PacMan->setResetParams();
    }
    else {
        PacMan = new class PacMan({12, 19}, TileSize, "assets/pacman40x160.png", 'N');
    }

    if (Ghosts.empty()){

        Ghosts.push_back(new Ghost({12, 10}, TileSize, "redDot40x40.png", 'N', &PathIds, &MapText));
        Ghosts.push_back(new Ghost({12, 9}, TileSize, "greenDot40x40.png", 'N', &PathIds, &MapText));
        Ghosts.push_back(new Ghost({12, 11}, TileSize, "cyanDot40x40.png", 'N', &PathIds, &MapText));
        Ghosts.push_back(new Ghost({14, 10}, TileSize, "magentaDot40x40.png", 'N', &PathIds, &MapText));

    } else {
        for (auto& ghost : Ghosts) { ghost->setResetParams(); }
    }

    auto visited = Ghosts[0]->checkVisited(MapText);
    for (auto& g : Ghosts) {
        g->Visited = visited;
    }

    ThePauseMenu->InGameplay = true;
    ScoreDisplay = new NumberDisplay(Font, TileSize / 4 * 3, {TileSize * 3, TileSize / 5 * 2});
    LifesDisplay = new NumberDisplay(Font, TileSize / 4 * 3, {TileSize * 3, TileSize * 23});
    UpgradeOn = false;
    UpgradeTimer = 0;
    EatenCount = 0;
    Score = 0;
    Lifes = 3;
    createMap();
    FruitNotYetAppeared = true;
    StartNumOfPoints = StationaryObjs.size();
}

void Game::update() {
    pullEvent();
    checkUpgradeTime();
    ThePauseMenu->checkPause();
    if (!ThePauseMenu->InGameplay) {
        TheMainMenu->InGameplay = ThePauseMenu->InGameplay;
    }

    if (ThePauseMenu->GamePaused) {
        ThePauseMenu->pauseScreen();
    } else {
        PacMan->tryMove(Map, TileSize);

        std::thread thStationaryObjsCollision(&Game::checkStationaryObjsCollision, this);

        std::vector<std::future<void>> futures;
        for (const auto& ghost : Ghosts) {
            futures.push_back(std::async(std::launch::async, &Ghost::checkAndMove, ghost));
        }

        for (auto& f : futures) { f.wait(); }
        thStationaryObjsCollision.join();
        checkFruitAppearance();

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
                    if (--Lifes == 0) { gameplayHasEnded(); }
                    else {
                        setStartPos();
                        upgradeOff();
                    }
                }
            }
        }

        ScoreDisplay->text.setString("1UP\n" + std::to_string(Score));
        LifesDisplay->text.setString("LIFE'S: " + std::to_string(Lifes));

        if (StationaryObjs.empty()) {
            gameplayHasEnded();
        }
    }

    pullEvent();
}

void Game::render() {
    if (TheMainMenu->InGameplay) {
        Window->clear(sf::Color::Black);

        drawMap();
        drawStationaryObjs();

        Window->draw(PacMan->Sprite);

        drawGhosts();

        Window->draw(ScoreDisplay->text);
        Window->draw(LifesDisplay->text);
        drawPortals();

        if (ThePauseMenu->GamePaused) { drawPauseMenu(); }

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

void Game::createMap() {
    std::vector<sf::Vector2<float>> portalPos;
    StationaryObjs.clear();
    Map.clear();

    for (int y = 0; y < MapText.size(); ++y) {
        std::vector<Tile*> row;
        for (int x = 0; x < MapText[y].size(); ++x)
            switch(MapText[y][x]) {
                case '#':
                    row.push_back(new Wall(TileSize, x, y));
                    break;
                case ' ':
                    row.push_back(new Path(TileSize, x, y)); //PathIds.push_back({x, y}); cant be here so ghost won't target their base
                    break;
                case '.':
                    row.push_back(new Path(TileSize, x, y));
                    PathIds.push_back({x, y});
                    StationaryObjs.push_back(new Point(4, sf::Vector2<float>(TileSize * x + TileSize/ 2.0, TileSize * y + TileSize / 2.0)));
                    break;
                case '*':
                    row.push_back(new Path(TileSize, x, y));
                    PathIds.push_back({x, y});
                    StationaryObjs.push_back(new Upgrade(12, sf::Vector2<float>(TileSize * x + TileSize/ 2.0, TileSize * y + TileSize / 2.0)));
                    break;
                case 'P':
                    row.push_back(new Portal(TileSize, x, y));
                    portalPos.push_back(sf::Vector2<float>(TileSize * x + TileSize/ 2.0, TileSize * y + TileSize / 2.0));
                    PortalID.push_back({x, y});
                    break;
                case '$':
                    row.push_back(new Gate(TileSize, x, y));
                    break;
                default:
                    row.push_back(new Background(TileSize, x, y));
            }

        Map.emplace_back(row);
    }

    if (portalPos.size() == 2) {
        std::cout << portalPos[1].x << " " << portalPos[1].y << std::endl;
        Map[PortalID[0].second][PortalID[0].first] = new Portal(portalPos[1], TileSize, PortalID[0].first, PortalID[0].second);
        Map[PortalID[1].second][PortalID[1].first] = new Portal(portalPos[0], TileSize, PortalID[1].first, PortalID[1].second);
    }
}

void Game::drawMap() {
    for (int y = 0; y < MapText.size(); ++y) {
        for (int x = 0; x < MapText[y].size(); ++x) {
            Window->draw(Map[y][x]->getBlock());
        }
    }
}

void Game::drawStationaryObjs() {
    for (auto& obj : StationaryObjs) {
        Window->draw(obj->getSprite());
    }
}

void Game::drawPortals() {
    for (const auto& id : PortalID) {
        Window->draw(Map[id.second][id.first]->getBlock());
    }
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
    for (auto iter = StationaryObjs.begin(); iter != StationaryObjs.end(); ++iter) {
        if (checkCollision(PacMan->Sprite.getPosition(),TileSize, iter.operator*()->getPosition(), iter.operator*()->getSize())) {
            std::pair<int, bool> scoreAndIsUpgrade = iter.operator*()->interact();

            delete iter.operator*();
            StationaryObjs.erase(iter);

            Score += scoreAndIsUpgrade.first;
            if (scoreAndIsUpgrade.second) {
                UpgradeOn = true;
                UpgradeTimer = Fps * 9;
                UpgradeBlinkCouter = Fps / 5;
                for (auto& g : Ghosts) {
                    g->requestSpeedChange(0.5f);
                }
            }
        }
    }
}

void Game::deleteMap() {
    for (auto& row : Map) {
        for (auto& tile : row) {
            delete tile;
        }
        row.clear();
    }
    Map.clear();
}

void Game::deleteStationaryObjs() {
    for (auto& obj : StationaryObjs) {
        delete obj;
    }
    StationaryObjs.clear();
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
            sf::Sprite temp = ghost->Sprite;
            if (ghost->Eaten) {
                temp.setTexture(GhostTextureEaten.getTexture());
                Window->draw(temp);
            } else if (!ghost->EatenAtCurrUpgrade and !ghost->Eaten) {
                if (statBlinking) {
                    if (UpgradeBlinkCouter < Fps / 10) {
                        temp.setTexture(GhostTextureUpgrade.getTexture());
                        Window->draw(temp);
                        if (UpgradeBlinkCouter == 0) { UpgradeBlinkCouter = Fps / 5; }
                    }
                } else {
                    temp.setTexture(GhostTextureUpgrade.getTexture());
                    Window->draw(temp);
                }
            } else {
                Window->draw(ghost->Sprite);
            }
        }
    } else {
        for (const auto& ghost : Ghosts) {
            if (ghost->Eaten) {
                sf::Sprite temp = ghost->Sprite;
                temp.setTexture(GhostTextureEaten.getTexture());
                Window->draw(temp);
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

void Game::loadTexture() {
    sf::CircleShape circle(TileSize / 2.0);

    GhostTextureEaten.create(TileSize, TileSize);
    GhostTextureEaten.clear(sf::Color::Transparent);
    circle.setFillColor(sf::Color(255, 255, 255, 200));
    GhostTextureEaten.draw(circle);

    GhostTextureUpgrade.create(TileSize, TileSize);
    GhostTextureUpgrade.clear(sf::Color::Transparent);
    circle.setFillColor(sf::Color::Blue);
    GhostTextureUpgrade.draw(circle);
}

void Game::upgradeOff() {
    UpgradeOn = false;
    EatenCount = 0;
    for (auto& g : Ghosts) {
        g->requestSpeedChange(1.0f);
        g->EatenAtCurrUpgrade = false;
    }
}
//todo dorobić znikanie owoców po X czasie
void Game::checkFruitAppearance() {
    if (FruitNotYetAppeared and StartNumOfPoints - 100 >= StationaryObjs.size()) {
        FruitNotYetAppeared = false;
        //position is under ghost base = 12 13
        StationaryObjs.push_back(new Fruit(20, sf::Vector2<float>(TileSize * 12 + TileSize/ 2.0, TileSize * 13 + TileSize / 2.0)));
    }
}