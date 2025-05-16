#include "gameWindow.h"
//
// Created by Sefa on 5/16/2025.
//
GameWindow::GameWindow() {
    window = nullptr;
    renderer = nullptr;
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Pathfinder Game");
}