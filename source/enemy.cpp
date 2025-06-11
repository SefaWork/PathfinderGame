#include "../include/enemy.h"
#include <iostream>

Enemy::Enemy(int startTileX, int startTileY, int tileSize, int moveDelay)
    : xTile(startTileX),
      yTile(startTileY),
      tileSize(tileSize),
      movementStrategy(nullptr),
      moveTimer(0),
      currentMoveDelay(moveDelay > 0 ? moveDelay : 60) {

    std::cout << "Enemy created ... with currentMoveDelay = " << currentMoveDelay << std::endl;
    xPos = xTile * tileSize;
    yPos = yTile * tileSize;
    width = tileSize;
    height = tileSize;

    std::cout << "Enemy created at tile (" << xTile << ", " << yTile << ")"
              << " with currentMoveDelay = " << currentMoveDelay << std::endl;
}

Enemy::~Enemy() {
    delete movementStrategy;
}

void Enemy::setMovementStrategy(IMovementStrategy* strategy) {
    delete movementStrategy;
    movementStrategy = strategy;
}

void Enemy::setMoveDelay(int delay) {
    currentMoveDelay = (delay > 0) ? delay : 1;
    moveTimer = 0;
}

void Enemy::update(const Map& gameMap, const SDL_Point& playerPos) {
    moveTimer++;

    if (moveTimer >= currentMoveDelay) {
        moveTimer = 0;

        if (movementStrategy) {
            SDL_Point currentPos = {xTile, yTile};
            SDL_Point nextMoveTile = movementStrategy->getNextMovement(currentPos, playerPos, gameMap);

            if (nextMoveTile.x != xTile || nextMoveTile.y != yTile) {
                if (nextMoveTile.x >= 0 && nextMoveTile.x < gameMap.getWidth() &&
                    nextMoveTile.y >= 0 && nextMoveTile.y < gameMap.getHeight() &&
                    gameMap.getTile(nextMoveTile.x, nextMoveTile.y).isTraversable()) {

                    xTile = nextMoveTile.x;
                    yTile = nextMoveTile.y;
                    xPos = xTile * tileSize;
                    yPos = yTile * tileSize;
                }
            }
        }
    }
}

void Enemy::draw(SDL_Renderer* renderer) {
    if (renderer == nullptr) return;
    SDL_Rect enemyRect = {xPos, yPos, width, height};
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderFillRect(renderer, &enemyRect);
}