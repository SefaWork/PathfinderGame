#ifndef ENEMY_H
#define ENEMY_H

#include "map.h"
#include "movementStrategy.h"

class Enemy {
public:
    Enemy(int startTileX, int startTileY, int tileSize, int moveDelay = 60);
    ~Enemy();

    void update(const Map& map, const SDL_Point& playerPos, const WavefrontMap* wavefrontMap);
    void draw(SDL_Renderer* renderer);
    void setMovementStrategy(IMovementStrategy* strategy);
    void setMoveDelay(int delay);

    [[nodiscard]] int getTileX() const {return xTile;}
    [[nodiscard]] int getTileY() const {return yTile;}

    [[nodiscard]] int getX() const {return xPos;}
    [[nodiscard]] int getY() const {return yPos;}
private:
    int xPos;
    int yPos;

    int xTile;
    int yTile;

    int tileSize;
    int width;
    int height;

    IMovementStrategy* movementStrategy;

    int moveTimer;
    int currentMoveDelay;
};

#endif //ENEMY_H
