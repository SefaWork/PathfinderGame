#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <SDL_rect.h>
#include <string>
#include <vector>

namespace EnemyAI {
    enum class Behavior {
        WAVEFRONT,
        RANDOM,
        ASTAR_PURSUE
    };
}

struct EnemySpawnInfo {
    int startX;
    int startY;
    EnemyAI::Behavior behavior;
    int moveDelay;
};

struct LevelConfig {
    std::string filePath;
    SDL_Point startPos;
    SDL_Point exitPos;
    std::vector<EnemySpawnInfo> enemies;
};

#endif //LEVELCONFIG_H
