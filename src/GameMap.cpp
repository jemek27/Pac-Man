//
// Created by jemek on 24.07.2024.
//

#include "GameMap.h"

GameMap::GameMap(const int tileSize, const int fruitValue) : TileSize(tileSize), FruitValue(fruitValue) {
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
}

GameMap::~GameMap() {
    deleteMap();
    deleteStationaryObjs();
}

void GameMap::createMap() {
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
        delete Map[PortalID[0].second][PortalID[0].first];
        Map[PortalID[0].second][PortalID[0].first] = new Portal(portalPos[1], TileSize, PortalID[0].first,
                                                                PortalID[0].second, MapSprites["empty_space"]);
        delete Map[PortalID[1].second][PortalID[1].first];
        Map[PortalID[1].second][PortalID[1].first] = new Portal(portalPos[0], TileSize, PortalID[1].first,
                                                                PortalID[1].second, MapSprites["empty_space"]);
    }
}

void GameMap::loadMapTextures() {
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

sf::Sprite GameMap::wallMatching(const int& x, const int& y) {
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

char GameMap::checkNeighbourWalls(const int &x, const int &y) {
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

sf::Sprite GameMap::gateMatching(const int& x, const int& y) {
    sf::Sprite gate = MapSprites["gate"];
    char dir_NSWE_Neighbours = checkNeighbourWalls(x, y);

    if (dir_NSWE_Neighbours == 0b1100) { gate.rotate(90); }
    return gate;
}

void GameMap::drawMap(sf::RenderWindow* Window) {
    for (int y = 0; y < MapText.size(); ++y) {
        for (int x = 0; x < MapText[y].size(); ++x) {
            Window->draw(Map[y][x]->getBlock());
        }
    }
}

void GameMap::drawStationaryObjs(sf::RenderWindow* Window) {
    for (auto& obj : StationaryObjs) {
        Window->draw(obj.second->getSprite());
    }
}

void GameMap::drawPortals(sf::RenderWindow* Window) {
    for (const auto& id : PortalID) {
        Window->draw(Map[id.second][id.first]->getBlock());
    }
}

void GameMap::deleteMap() {
    for (auto& row : Map) {
        for (auto& tile : row) {
            delete tile;
        }
        row.clear();
    }
    Map.clear();
}

void GameMap::deleteStationaryObjs() {
    for (auto& obj : StationaryObjs) {
        delete obj.second;
    }
    StationaryObjs.clear();
}