#include <iostream>

#include "game/game.h"

int main(int argc, char* argv[]) {
    // Get our Game singleton instance.
    Game* game = Game::getInstance();

    // Run the game loop and wait for it to exit.
    return game->gameLoop();
}
