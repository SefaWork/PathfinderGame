#ifndef ENEMY_H
#define ENEMY_H

#include "map.h"
#include "movementStrategy.h"

///
/// Class representing enemy objects.
///
class Enemy {
public:
    Enemy(int startTileX, int startTileY, int tileSize, int moveDelay = 60);
    ~Enemy();

    void update(const Map& map, const SDL_Point& playerPos);
    void draw(SDL_Renderer* renderer);

    /// Sets the enemy's movement strategy which defines how it should move every movement opportunity.
    /// @param strategy Pointer to the new strategy.
    void setMovementStrategy(IMovementStrategy* strategy);

    /// Sets the delay (in frames) that the enemy is allowed to move.
    /// @param delay In frames (60 = 1 second)
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
