#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[]) {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);

    SDL_Delay(10000);

    SDL_Quit();

    return 0;
}