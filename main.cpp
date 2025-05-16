#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[]) {
    bool running = true;

    SDL_Event event;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    SDL_Quit();

    return 0;
}