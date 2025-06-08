#include "../../include/movement/wavefrontChase.h"
#include <iostream>
#include <climits>

WavefrontChaseMovementStrategy::WavefrontChaseMovementStrategy() = default;

SDL_Point WavefrontChaseMovementStrategy::getNextMovement(const SDL_Point& currentPos, const SDL_Point& targetPos, const Map& gameMap, const WavefrontMap* wavefrontMap) {
    if (!wavefrontMap || wavefrontMap->empty() || (*wavefrontMap)[0].empty()) {
        std::cerr << "WavefrontFollowStrategy: Wavefront map is not available or empty for enemy at ("
                  << currentPos.x << "," << currentPos.y << ")" << std::endl;
        return currentPos;
    }

    const WavefrontMap& wfMap = *wavefrontMap;
    int mapHeight = wfMap.size();
    int mapWidth = wfMap[0].size();

    if (currentPos.y < 0 || currentPos.y >= mapHeight || currentPos.x < 0 || currentPos.x >= mapWidth) {
        std::cerr << "WavefrontFollowStrategy: Enemy at (" << currentPos.x << "," << currentPos.y
                  << ") is out of wavefront map bounds." << std::endl;
        return currentPos;
    }

    int currentWaveValue = wfMap[currentPos.y][currentPos.x];

    if (currentWaveValue == 0) {
        return currentPos;
    }

    if (currentWaveValue == -1 || currentWaveValue == INT_MAX) {
        std::cout << "WavefrontFollowStrategy: Enemy at (" << currentPos.x << "," << currentPos.y
                  << ") is on an UNREACHABLE or UNSET wave tile (value: " << currentWaveValue
                  << "). Cannot move. Target is (" << targetPos.x << "," << targetPos.y << ")." << std::endl;
        return currentPos; // Hareket etme
    }

    SDL_Point bestNextPos = currentPos;
    int minNeighborWaveValue = currentWaveValue;

    SDL_Point directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // Yuk, As, Sol, Sag

    for (const auto& dir : directions) {
        SDL_Point neighborPos = {currentPos.x + dir.x, currentPos.y + dir.y};

        if (neighborPos.x >= 0 && neighborPos.x < mapWidth &&
            neighborPos.y >= 0 && neighborPos.y < mapHeight &&
            gameMap.getTile(neighborPos.x, neighborPos.y).isTraversable()) {

            int neighborWaveValue = wfMap[neighborPos.y][neighborPos.x];

            if (neighborWaveValue != -1 && neighborWaveValue < minNeighborWaveValue) {
                minNeighborWaveValue = neighborWaveValue;
                bestNextPos = neighborPos;
            }
        }
    }

    // Eğer hiçbir komşu mevcut dalga değerinden daha iyi değilse (ve hedefte değilsek), sıkışmışızdır.
    if (bestNextPos.x == currentPos.x && bestNextPos.y == currentPos.y && currentWaveValue != 0) {
         std::cout << "WavefrontFollowStrategy: Enemy at (" << currentPos.x << "," << currentPos.y
                   << ") is STUCK. Current wave: " << currentWaveValue
                   << ". Target: (" << targetPos.x << "," << targetPos.y << ")." << std::endl;
    }

    return bestNextPos;
}