//
// Created by Sefa on 5/16/2025.
//

#ifndef GAME_H
#define GAME_H

#include "game.h"
#include "gameWindow.h"

class Game {
public:
    static Game *getInstance();
    int gameLoop();
private:
    static Game *instance;

    GameWindow *window;
    bool active;

    Game();
};

#endif //GAME_H
