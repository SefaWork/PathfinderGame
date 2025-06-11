#ifndef COIN_H
#define COIN_H

#include <SDL.h>
#include "map.h"

class Coin {
public:
    Coin(int posX, int posY, int tileSize);

    void update(const SDL_Point& playerPos);
    void draw(SDL_Renderer* renderer);
private:
    int posX;
    int posY;
    int tileSize;
    bool active;

    SDL_Rect rect;
};

#endif //COIN_H
