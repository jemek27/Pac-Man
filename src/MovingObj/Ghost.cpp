//
// Created by jemek on 21.06.2024.
//

#include "MovingObj/Ghost.h"

Ghost::Ghost(std::pair<int, int> startTileId, int tileSize, const std::string &fileName,
             char starDir, std::vector<std::pair<int, int>>* possibleTargets, std::vector<std::string>* textMap)
             : MovingObj(startTileId, tileSize, fileName, starDir), GhostID(fileName[0]), TargetId(startTileId), ResetId({12, 10}),
             Eaten(false), EatenAtCurrUpgrade(false), Speed(1.0f), PossibleTargets(possibleTargets), TextMap(textMap),
             DirMoveCounter(0), SavedNumForCounter(tileSize/Step), TileSize(tileSize), NumForCounter(SavedNumForCounter),
             RequestedSpeed(1.0f), MoveCounterMultiplier(1.0f){

    Sprite = sf::Sprite();

    if (!texturePng.loadFromFile(fileName)) {
        std::cerr << fileName + " couldn't be read\n";
        // Creating a replacement Texture

        sf::CircleShape circle(tileSize / 2.0);

        switch (fileName[0]) {
            case 'r' :
                circle.setFillColor(sf::Color::Red);
                break;
            case 'g' :
                circle.setFillColor(sf::Color::Green);
                break;
            case 'c' :
                circle.setFillColor(sf::Color::Cyan);
                break;
            case 'm' :
                circle.setFillColor(sf::Color::Magenta);
                break;
        }


        Texture.create(tileSize, tileSize);
        Texture.clear(sf::Color::Transparent);
        Texture.draw(circle);
        Texture.display();

        Sprite.setTexture(Texture.getTexture());

    } else {
        ImageLoaded = true;
        ImagePosition = sf::IntRect(0,0,40,40);
        Sprite = sf::Sprite(texturePng, ImagePosition);
        rotateImageToDir();
        Sprite.setTextureRect(ImagePosition);
    }

    Sprite.setOrigin(tileSize / 2.0, tileSize / 2.0);
    // + tileSize / 2.0 because of the origin
    Sprite.setPosition(tileSize * startTileId.first + tileSize / 2.0, tileSize * startTileId.second + tileSize / 2.0);
}

void Ghost::wasEaten() {
    Eaten = true;


    checkTileIds(TileSize);

    if (PosTileIDs.second_x != -1) { //between tiles
        char stepsToSingleTile = NumForCounter - DirMoveCounter;
        TargetId = {PosTileIDs.first_x, PosTileIDs.first_y};
//        std::cout << __LINE__ << " " << PosTileIDs.first_y << " " << PosTileIDs.first_x << std::endl;
        findRoute();
        CurrentDirection = RouteDir.front();

        if (ImageLoaded) {
            rotateImageToDir();
            Sprite.setTextureRect(ImagePosition);
        }

        RouteDir.pop_front();
        //    std::cout << __LINE__ << " |"<< PosTileIDs.first_x * 40 << ' ' << PosTileIDs.first_y * 40  << std::endl;
//    std::cout << __LINE__ << " |"<< Sprite.getPosition().x << ' ' << Sprite.getPosition().y   << std::endl;
        DirMoveCounter = stepsToSingleTile;
//    std::cout << __LINE__ << " |"<< int(stepsToSingleTile) << std::endl;
    }
}

void Ghost::checkAndMove() {
    checkTileIds(TileSize);

    if (RequestedSpeed != Speed and PosTileIDs.second_x == -1) {
        Speed = RequestedSpeed;
        NumForCounter *= MoveCounterMultiplier;
    }

    if (Eaten and PosTileIDs.first_x == ResetId.first and
        PosTileIDs.first_y == ResetId.second) {
        Eaten = false;
        Speed = 1.0f;
        RequestedSpeed = 1.0f;
        MoveCounterMultiplier = 1.0f;
        NumForCounter = SavedNumForCounter;
    }

    if(RouteDir.empty() and DirMoveCounter == 0) {
        if (Eaten) {
            requestSpeedChange(2.0f);
            TargetId = ResetId;
        } else {
            selectRandomTarget();
        }

        findRoute();
        CurrentDirection = RouteDir.front();

        if (ImageLoaded) {
            rotateImageToDir();
            Sprite.setTextureRect(ImagePosition);
        }

        RouteDir.pop_front();
        DirMoveCounter = NumForCounter;
    }

    if (DirMoveCounter == 0) {
        CurrentDirection = RouteDir.front();

        if (ImageLoaded) {
            rotateImageToDir();
            Sprite.setTextureRect(ImagePosition);
        }

        RouteDir.pop_front();
        DirMoveCounter = NumForCounter;
    }

    --DirMoveCounter;
    move(Speed);
}

void Ghost::findRoute() {
    QItem source(0, 0, std::deque<char>());
    std::vector<std::string> map = *TextMap;
    checkTileIds(TileSize);
//    std::string ghostLog;
//    ghostLog.push_back(GhostID);
//    ghostLog += "\n";
    if (PosTileIDs.second_x != -1) { //between tiles
        map[PosTileIDs.second_y][PosTileIDs.second_x] = 's';
        source.row = PosTileIDs.second_y;
        source.col = PosTileIDs.second_x;
//        ghostLog += std::to_string(__LINE__) + " |" + std::to_string(PosTileIDs.second_y) + " " + std::to_string(PosTileIDs.second_x) + "\n";
    } else {
        map[PosTileIDs.first_y][PosTileIDs.first_x] = 's';
        source.row = PosTileIDs.first_y;
        source.col = PosTileIDs.first_x;
//        ghostLog += std::to_string(__LINE__) + " |" + std::to_string(PosTileIDs.first_y) + " " + std::to_string(PosTileIDs.first_x) + "\n";
    }
//    ghostLog += std::to_string(__LINE__) + " |" + std::to_string(TargetId.second) + " " + std::to_string(TargetId.first) + "\n";
    map[TargetId.second][TargetId.first] = 'd';
//    for (auto r : map) {
//        ghostLog += r + "\n";
//    }
//    std::cout << ghostLog << "\n\n" << std::endl;
    std::vector<std::vector<bool>> visited = Visited;
    // applying BFS on matrix cells starting from source
    std::queue<QItem> q;
    q.push(source);
    visited[source.row][source.col] = true;
    while (!q.empty()) {
        QItem p = q.front();
        q.pop();
        auto temp = p.dir;


        if (map[p.row][p.col] == 'd') { // Destination found
            RouteDir = p.dir;
            break;
        }

        if (p.row - 1 >= 0 and !visited[p.row - 1][p.col]) {
            temp.push_back('N');
            q.emplace(p.row - 1, p.col, temp);
            temp.pop_back();
            visited[p.row - 1][p.col] = true;
        }

        if (p.row + 1 < map[0].size() and !visited[p.row + 1][p.col]) {
            temp.push_back('S');
            q.emplace(p.row + 1, p.col, temp);
            temp.pop_back();
            visited[p.row + 1][p.col] = true;
        }

        if (p.col - 1 >= 0 and !visited[p.row][p.col - 1]) {
            temp.push_back('W');
            q.emplace(p.row, p.col - 1, temp);
            temp.pop_back();
            visited[p.row][p.col - 1] = true;
        }

        if (p.col + 1 < map.size() and !visited[p.row][p.col + 1]) {
            temp.push_back('E');
            q.emplace(p.row, p.col + 1, temp);
            temp.pop_back();
            visited[p.row][p.col + 1] = true;
        }
    }
}


void Ghost::selectRandomTarget() {
    checkTileIds(TileSize);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, PossibleTargets->size() - 1); // Define the range
    int randomIndex = distr(gen);

    do {
        TargetId = PossibleTargets->at(randomIndex);
    } while (PosTileIDs.first_x == TargetId.first and PosTileIDs.first_y == TargetId.second);

//    std::cout << TargetId.first << " " << TargetId.second << std::endl;
}

void Ghost::setResetParams() {
    setStartPos();
    DirMoveCounter = 0;
    Eaten = false;
    EatenAtCurrUpgrade = false;
    Speed = 1.0f;
    RequestedSpeed = 1.0f;
    NumForCounter = SavedNumForCounter;
    MoveCounterMultiplier = 1.0f;
    RouteDir.clear();
}

std::vector<std::vector<bool>> Ghost::checkVisited(const std::vector<std::string>& map) {
    std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size()));
    for (int i = 0; i < map[0].size(); i++) {
        for (int j = 0; j < map.size(); j++) {
            if (map[i][j] == '#' or map[i][j] == 'B')
                visited[i][j] = true;
            else
                visited[i][j] = false;
        }
    }
    return visited;
}

void Ghost::requestSpeedChange(const float &speed) {
    RequestedSpeed = speed;
    MoveCounterMultiplier = Speed / RequestedSpeed;
}
//todo do mov obj
void Ghost::rotateImageToDir() {
    switch (CurrentDirection){
        case 'N':
            ImagePosition.left = 0;
            break;
        case 'S':
            ImagePosition.left = 40;
            break;
        case 'E':
            ImagePosition.left = 80;
            break;
        case 'W':
            ImagePosition.left = 120;
            break;
    }
}