
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL_events.h>
#include "map.h"

class Player {
public:
    Player(int tilex, int tiley, int tileSize);

    void handleEvent(const SDL_Event& event);
    void update(const Map& gameMap);
    void draw(SDL_Renderer* renderer);

    // Getters.
    [[nodiscard]] int getX() const {return xPos;}
    [[nodiscard]] int getY() const {return yPos;}
    [[nodiscard]] int getTileX() const {return xTile;}
    [[nodiscard]] int getTileY() const {return yTile;}
private:
    int xPos;
    int yPos;

    int xTile;
    int yTile;
    int tileSize;

    int width;
    int height;

    int velX;
    int velY;
};

#endif //PLAYER_H
