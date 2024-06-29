
#include "Game.h"

//todo exit po rozpoczeciu rozgrywki
//todo koncept TileSize % mov == 0
//todo zamiana nicku
int main()
{
    int windowWidth = 1000;
    int windowHeight = 1000;
    int fps = 60;

    Game* game = new Game(windowHeight, windowWidth, fps, "Pac-Man");


    while (game->running()) {
        game->drawMainMenu();
        game->actInMainMenu();
        if (game->getInGameplay()) {
            game->initiateGamePlay();
            while (game->getInGameplay()) {
                game->update();
                game->render();
            }
        }
    }
    std::cout << "END";
    return 0;
}
