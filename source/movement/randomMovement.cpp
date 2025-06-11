#include "../../include/movement/randomMovement.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

RandomMovementStrategy::RandomMovementStrategy() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng = std::mt19937(seed);
}

SDL_Point RandomMovementStrategy::getNextMovement(const SDL_Point& currentPos, const SDL_Point& targetPos, const Map& gameMap) {
    SDL_Point directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    std::vector<SDL_Point> validMoves;

    int mapWidth = gameMap.getWidth();
    int mapHeight = gameMap.getHeight();

    for (const auto& dir : directions) {
        SDL_Point nextPos = {currentPos.x + dir.x, currentPos.y + dir.y};

        if (nextPos.x >= 0 && nextPos.x < mapWidth &&
            nextPos.y >= 0 && nextPos.y < mapHeight &&
            gameMap.getTile(nextPos.x, nextPos.y).isTraversable()) {
            validMoves.push_back(nextPos);
            }
    }

    if (validMoves.empty()) {
        return currentPos;
    }

    std::shuffle(validMoves.begin(), validMoves.end(), rng);
    return validMoves[0];
}