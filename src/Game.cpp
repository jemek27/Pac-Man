//
// Created by jemek on 10.05.2024.
//

#include "Game.h"
//todo walltextures
//todo pacman animation
Game::Game(int windowH, int windowW, int fps, const std::string& name)
            : VideoMode(windowH, windowW), Fps(fps), FrameCounter(0), TextureShiftFrameThreshold(Fps / 6),
            BlinkCouterThreshold(Fps / 10), ScoreDisplay(nullptr), AnimationCount(0),
            LivesDisplay(nullptr), PacMan(nullptr), Score(0), UpgradeOn(false), StartNumOfPoints(0),
            FruitNotYetAppeared(true), UpgradeTimer(0), EatenCount(0), Lives(3), UpgradeBlinkCouter(0) {

    Window = new sf::RenderWindow(VideoMode, name);
    Window->setFramerateLimit(fps);

    if (!Font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Failed to load Font file!" << std::endl;
    }

    loadMapTextures();

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

void Game::loadMapTextures() {
    if (!MapTextures["wallCorner"].loadFromFile("assets/mapa_pacman_wall_corner.png")) {
        std::cerr << "assets/assets/mapa_pacman_wall_corner.png not loaded" << std::endl;
    }
    if (!MapTextures["gate"].loadFromFile("assets/mapa_pacman_gate.png")) {
        std::cerr << "assets/mapa_pacman_gate.png not loaded" << std::endl;
    }
    if (!MapTextures["emptySpace"].loadFromFile("assets/mapa_pacman_empty_space.png")) {
        std::cerr << "assets/mapa_pacman_empty_space.png not loaded" << std::endl;
    }
    if (!MapTextures["wall"].loadFromFile("assets/mapa_pacman_wall.png")) {
        std::cerr << "assets/assets/mapa_pacman_wall.png not loaded" << std::endl;
    }
    if (!MapTextures["threeWay"].loadFromFile("assets/mapa_pacman_three_way.png")) {
        std::cerr << "assets/mapa_three_way.png not loaded" << std::endl;
    }
    if (!MapTextures["fourWay"].loadFromFile("assets/mapa_pacman_four_way.png")) {
        std::cerr << "assets/mapa_pacman_four_way.png not loaded" << std::endl;
    }
    if (!MapTextures["wallEnd"].loadFromFile("assets/mapa_pacman_wall_end.png")) {
        std::cerr << "assets/mapa_pacman_wall_end.png not loaded" << std::endl;
    }
    if (!MapTextures["wallLone"].loadFromFile("assets/mapa_pacman_wall_lone.png")) { //sad pepe
        std::cerr << "assets/mapa_pacman_wall_lone.png not loaded" << std::endl;
    }

    MapSprites["empty_space"] = sf::Sprite (MapTextures["emptySpace"]);
    MapSprites["wallCorner"] = sf::Sprite (MapTextures["wallCorner"]);
    MapSprites["gate"] = sf::Sprite (MapTextures["gate"]);
    MapSprites["wall"] = sf::Sprite (MapTextures["wall"]);
    MapSprites["threeWay"] = sf::Sprite (MapTextures["threeWay"]);
    MapSprites["fourWay"] = sf::Sprite (MapTextures["fourWay"]);
    MapSprites["wallEnd"] = sf::Sprite (MapTextures["wallEnd"]);
    MapSprites["wallLone"] = sf::Sprite (MapTextures["wallLone"]);
}

//todo raz wczytać tekstury do programu (cha trzeba od razu Sprite zrobić)
void Game::initiateGamePlay() {
    if (!Map.empty()) { deleteMap(); }
    if (!StationaryObjs.empty()) { deleteStationaryObjs(); }
    createMap();

    if (PacMan != nullptr) {
        PacMan->setPosTileIDs({12, 19}, {-1, -1});
        PacMan->setResetParams();
    }
    else {
        PacMan = new class PacMan({12, 19}, TileSize, "assets/pacman40x160.png", 'N');
    }

    if (Ghosts.empty()){

        Ghosts.push_back(new Ghost({12, 10}, TileSize, "assets/redGhostAnimated_80x160.png", 'N', &PathIds, &MapText));
        Ghosts.push_back(new Ghost({12, 9}, TileSize, "assets/greenGhostAnimated_80x160.png", 'N', &PathIds, &MapText));
        Ghosts.push_back(new Ghost({12, 11}, TileSize, "assets/cyanGhostAnimated_80x160.png", 'N', &PathIds, &MapText));
        Ghosts.push_back(new Ghost({14, 10}, TileSize, "assets/magentaGhostAnimated_80x160.png", 'N', &PathIds, &MapText));

    } else {
        for (auto& ghost : Ghosts) { ghost->setResetParams(); }
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

    auto visited = Ghosts[0]->checkVisited(MapText);
    for (auto& g : Ghosts) {
        g->Visited = visited;
        g->selectRandomTarget();
        g->findRoute();
    }

    FruitNotYetAppeared = true;
    StartNumOfPoints = StationaryObjs.size();
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
            checkTextureShift();
            PacMan->tryMove(Map, TileSize);

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

            if (StationaryObjs.empty()) {
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

        drawMap();
        drawStationaryObjs();

        Window->draw(PacMan->Sprite);

        drawGhosts();

        Window->draw(ScoreDisplay->text);
        Window->draw(LivesDisplay->text);
        drawPortals();

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

void Game::createMap() {
    std::vector<sf::Vector2<float>> portalPos;

    for (int y = 0; y < MapText.size(); ++y) {
        std::vector<Tile*> row;
        sf::Sprite tempTile;
        for (int x = 0; x < MapText[y].size(); ++x)
            switch(MapText[y][x]) {
                case '#':
                    tempTile = wallMatching(x, y);
                    row.push_back(new Wall(TileSize, x, y, tempTile));
                    break;
                case ' ':
                    row.push_back(new Path(TileSize, x, y, MapSprites["empty_space"])); //PathIds.push_back({x, y}); cant be here so ghost won't target their base
                    break;
                case '.':
                    row.push_back(new Path(TileSize, x, y, MapSprites["empty_space"]));
                    PathIds.push_back({x, y});
                    StationaryObjs[{x, y}] = new Point(4, sf::Vector2<float>(TileSize * x + TileSize / 2.0, TileSize * y + TileSize / 2.0));
                    break;
                case '*':
                    row.push_back(new Path(TileSize, x, y, MapSprites["empty_space"]));
                    PathIds.push_back({x, y});
                    StationaryObjs[{x, y}] = new Upgrade(12, sf::Vector2<float>(TileSize * x + TileSize / 2.0, TileSize * y + TileSize / 2.0));
                    break;
                case 'P':
                    row.push_back(new Portal(TileSize, x, y, MapSprites["empty_space"]));
                    portalPos.push_back(sf::Vector2<float>(TileSize * x + TileSize/ 2.0, TileSize * y + TileSize / 2.0));
                    PortalID.push_back({x, y});
                    break;
                case '$':
                    tempTile = gateMatching(x, y);
                    row.push_back(new Gate(TileSize, x, y, tempTile));
                    break;
                default:
                    row.push_back(new Background(TileSize, x, y, MapSprites["empty_space"]));
            }

        Map.emplace_back(row);
    }

    if (portalPos.size() == 2) {
        std::cout << portalPos[1].x << " " << portalPos[1].y << std::endl;
        Map[PortalID[0].second][PortalID[0].first] = new Portal(portalPos[1], TileSize, PortalID[0].first, PortalID[0].second, MapSprites["empty_space"]);
        Map[PortalID[1].second][PortalID[1].first] = new Portal(portalPos[0], TileSize, PortalID[1].first, PortalID[1].second, MapSprites["empty_space"]);
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
    //for (auto& obj : StationaryObjs) {
    //    Window->draw(obj->getSprite());
    //}

    for (auto& obj : StationaryObjs) {
        Window->draw(obj.second->getSprite());
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
    auto TileIDs = PacMan->getPosTileIDs();
    if (StationaryObjs.contains({TileIDs.second_x, TileIDs.second_y })) {
        std::vector<std::pair<int, int>> ids =  { { TileIDs.second_x, TileIDs.second_y } };

        if (StationaryObjs.contains({TileIDs.first_x, TileIDs.first_y })) {
            ids.push_back({ TileIDs.first_x, TileIDs.first_y });
        }

        for (auto id : ids) {
            if (checkCollision(PacMan->Sprite.getPosition(), TileSize, StationaryObjs[id]->getPosition(), StationaryObjs[id]->getSize())) {
                std::pair<int, bool> scoreAndIsUpgrade = StationaryObjs[id]->interact();

                delete StationaryObjs[id];
                StationaryObjs.erase(id);

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
        delete obj.second;
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

void Game::loadTexture() {
    if (!GhostEatenUpgradeTexturePng.loadFromFile("assets/upgradeEatenGhostAnimated_80x80.png")) {
//        sf::CircleShape circle(TileSize / 2.0);
//
//        GhostTextureEaten.create(TileSize, TileSize);
//        GhostTextureEaten.clear(sf::Color::Transparent);
//        circle.setFillColor(sf::Color(255, 255, 255, 200));
//        GhostTextureEaten.draw(circle);
//
//        GhostTextureUpgrade.create(TileSize, TileSize);
//        GhostTextureUpgrade.clear(sf::Color::Transparent);
//        circle.setFillColor(sf::Color::Blue);
//        GhostTextureUpgrade.draw(circle);
        std::cerr << "assets/upgradeEatenGhostAnimated_80x80.png not loaded" << std::endl;
    } else {
        ImagePosition = sf::IntRect(0,0,40,40);
        UpgradeGhost = sf::Sprite(GhostEatenUpgradeTexturePng, ImagePosition);
        UpgradeGhost.setTextureRect(ImagePosition);
        UpgradeGhost.setOrigin(TileSize / 2.0, TileSize / 2.0);
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
//todo dorobić znikanie owoców po X czasie
void Game::checkFruitAppearance() {
    if (FruitNotYetAppeared and StartNumOfPoints - 100 >= StationaryObjs.size()) {
        FruitNotYetAppeared = false;
        //position is under ghost base = 12 13
        if (StationaryObjs.contains({12, 13 })) { // if a point is there
            delete StationaryObjs[{12, 13 }];
            StationaryObjs[{12, 13 }] = new Fruit(20, sf::Vector2<float>(TileSize * 12 + TileSize / 2.0, TileSize * 13 + TileSize / 2.0), 250);
        } else {
            StationaryObjs[{12, 13 }] = new Fruit(20, sf::Vector2<float>(TileSize * 12 + TileSize / 2.0, TileSize * 13 + TileSize / 2.0), 200);
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

void Game::checkTextureShift() {
    if (++FrameCounter > TextureShiftFrameThreshold) {
        FrameCounter = 0;

        if (AnimationCount++ == 0) { //only to animation frames
            textureShift(40);
        } else {
            textureShift(0);
            AnimationCount = 0;
        }
    }
}

void Game::textureShift(const int topPos) {
    for (auto g : Ghosts) { g->setImagePositionAnimation(topPos); }

    ImagePosition.top = topPos;
    UpgradeGhost.setTextureRect(ImagePosition);
}

sf::Sprite Game::wallMatching(const int& x, const int& y) {
    sf::Sprite wall;
    char dir_NSWE_Neighbours = checkNeighbourWalls(x, y);

    switch (dir_NSWE_Neighbours){
        case 0b0000:
            wall = MapSprites["wallLone"];
            break;
        case 0b0001:
            wall = MapSprites["wallEnd"];
            wall.rotate(180);
            break;
        case 0b0010:
            wall = MapSprites["wallEnd"];
            break;
        case 0b0011:
            wall = MapSprites["wall"];
            break;
        case 0b0100:
            wall = MapSprites["wallEnd"];
            wall.rotate(270);
            break;
        case 0b0101:
            wall = MapSprites["wallCorner"];
            break;
        case 0b0110:
            wall = MapSprites["wallCorner"];
            wall.rotate(90);
            break;
        case 0b0111:
            wall = MapSprites["threeWay"];
            break;
        case 0b1000:
            wall = MapSprites["wallEnd"];
            wall.rotate(90);
            break;
        case 0b1001:
            wall = MapSprites["wallCorner"];
            wall.rotate(270);
            break;
        case 0b1010:
            wall = MapSprites["wallCorner"];
            wall.rotate(180);
            break;
        case 0b1011:
            wall = MapSprites["threeWay"];
            wall.rotate(180);
            break;
        case 0b1100:
            wall = MapSprites["wall"];
            wall.rotate(90);
            break;
        case 0b1101:
            wall = MapSprites["threeWay"];
            wall.rotate(270);
            break;
        case 0b1110:
            wall = MapSprites["threeWay"];
            wall.rotate(90);
            break;
        case 0b1111:
            wall = MapSprites["fourWay"];
            break;
        default:
            wall = MapSprites["wallLone"];
    }

    return wall;
}

char Game::checkNeighbourWalls(const int &x, const int &y) {
    char dir_NSWE_Neighbours = 0; // 0000

    if (y - 1 >= 0) {
        if (MapText[y - 1][x] == '#') { dir_NSWE_Neighbours |= 0b1000; }
    }
    if (x - 1 >= 0) {
        if (MapText[y][x - 1] == '#') { dir_NSWE_Neighbours |= 0b0010;}
    }
    if (y + 1 < MapText.size()) {
        if (MapText[y + 1][x] == '#') { dir_NSWE_Neighbours |= 0b0100;}
    }
    if (x + 1 < MapText[y].size()) {
        if (MapText[y][x + 1] == '#') { dir_NSWE_Neighbours |= 0b0001;}
    }
    return dir_NSWE_Neighbours;
}

sf::Sprite Game::gateMatching(const int& x, const int& y) {
    sf::Sprite gate = MapSprites["gate"];
    char dir_NSWE_Neighbours = checkNeighbourWalls(x, y);

    if (dir_NSWE_Neighbours == 0b1100) { gate.rotate(90); }
    return gate;
}
