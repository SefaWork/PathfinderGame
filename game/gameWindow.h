//
// Created by Sefa on 5/16/2025.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

class GameWindow {
public:
    GameWindow();
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif //WINDOW_H
