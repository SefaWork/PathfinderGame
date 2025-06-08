#include <iostream>
#include "../include/player.h"

Player::Player(int tilex, int tiley, int tileSize) {
    this->xTile = tilex;
    this->yTile = tiley;
    this->tileSize = tileSize;

    this->xPos = tilex * tileSize;
    this->yPos = tiley * tileSize;
    this->width = tileSize;
    this->height = tileSize;

    this->velX = 0;
    this->velY = 0;
}

void Player::handleEvent(const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:    velX =  0; velY = -1; break;
            case SDLK_DOWN:  velX =  0; velY =  1; break;
            case SDLK_LEFT:  velX = -1; velY =  0; break;
            case SDLK_RIGHT: velX =  1; velY =  0; break;
            default: break;
        }
    }
}

void Player::update(const Map& gameMap) {
    if (velX == 0 && velY == 0) {
        return;
    }

    int targetTileX = xTile + velX;
    int targetTileY = yTile + velY;

    if (targetTileX >= 0 && targetTileX < gameMap.getWidth() &&
        targetTileY >= 0 && targetTileY < gameMap.getHeight()) {

        const Tile& targetTile = gameMap.getTile(targetTileX, targetTileY);
        if (targetTile.isTraversable()) {
            xTile = targetTileX;
            yTile = targetTileY;
            xPos = xTile * tileSize;
            yPos = yTile * tileSize;
            std::cout << "Player moved to tile (" << xTile << ", " << yTile << ")" << std::endl;
        } else {
            std::cout << "Player tried to move to a non-traversable tile ("
                      << xTile << ", " << yTile << ")" << std::endl;
        }
        } else {
            std::cout << "Player tried to move out of map bounds to ("
                      << xTile << ", " << yTile << ")" << std::endl;
        }

    velX = 0;
    velY = 0;
}

void Player::draw(SDL_Renderer* renderer) {
    if (renderer == nullptr) return;

    SDL_Rect playerRect = {xPos, yPos, width, height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Sarı
    SDL_RenderFillRect(renderer, &playerRect);
}
