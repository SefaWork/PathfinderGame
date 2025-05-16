//
// Created by Sefa on 5/16/2025.
//

#include "game.h"
#include "gameWindow.h"
#include <SDL.h>

Game* Game::instance = nullptr;

Game::Game() {
    active = true;
    window = new GameWindow();
};

Game* Game::getInstance() {
    if (instance == nullptr) {
        instance = new Game();
    }

    return instance;
}

int Game::gameLoop() {
    SDL_Event event;
    while (this->active) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                this->active = false;
            }
        }
    }

    SDL_Quit();
    delete window;

    return 0;
}

