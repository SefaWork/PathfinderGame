#include "../include/coin.h"
#include "../include/gameManager.h"

class GameManager;

Coin::Coin(int posX, int posY, int tileSize): posX(posX), posY(posY), tileSize(tileSize), active(true) {
    int size = static_cast<int>(tileSize * 0.5f);
    int offset = static_cast<int>(size / 2);
    rect = SDL_Rect{posX * tileSize + offset, posY * tileSize + offset, size, size};
}

void Coin::draw(SDL_Renderer *renderer) {
    if (!active || renderer == nullptr) return;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Coin::update(const SDL_Point &playerPos) {
    if (!active) return;
    if (playerPos.x == posX && playerPos.y == posY) {
        active = false;
        GameManager::GetInstance()->getScore()->changeScore(25);
    }
}

