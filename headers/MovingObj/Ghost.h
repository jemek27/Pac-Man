//
// Created by jemek on 21.06.2024.
//

#ifndef PROJEKT_GHOST_H
#define PROJEKT_GHOST_H
#include "MovingObj.h"
#include <deque>
#include <queue>
#include <random>


//todo po zdjedzeniu i dotarciu do bazy znowu ustawienia zmienić emanie speed
//todo reset ogarnąć
//todo zjedzie gdy w kierunku poruszania jest ściano kończy się wejściem w scianę
class Ghost : public MovingObj {
    char GhostID;
    std::pair<int, int> TargetId;
    std::pair<int, int> ResetId;
    std::vector<std::string>* TextMap;
    std::vector<std::pair<int, int>>* PossibleTargets;
    std::vector<std::vector<bool>> Visited;
    std::deque<char> RouteDir;
    char DirMoveCounter;
    char SavedNumForCounter;
    char NumForCounter;
    bool Eaten;
    bool EatenAtCurrUpgrade;
    float Speed;
    float RequestedSpeed;
    float MoveCounterMultiplier;

    int TileSize;

    friend class Game;
    class QItem {
    public:
        int row;
        int col;
        std::deque<char> dir;
        QItem(int x, int y, std::deque<char> d)
                : row(x), col(y), dir(std::move(d)) {}
    };
public:
    Ghost(std::pair<int, int> startTileId, int tileSize, const std::string& fileName,
          char starDir, std::vector<std::pair<int, int>>* possibleTargets, std::vector<std::string>* textMap);
    void wasEaten();
    void checkAndMove();
    void findRoute();
    void selectRandomTarget();
    void setResetParams();
    void requestSpeedChange(const float& speed);
    std::vector<std::vector<bool>> checkVisited(const std::vector<std::string>& map);
};


#endif //PROJEKT_GHOST_H
