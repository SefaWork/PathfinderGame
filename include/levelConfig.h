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

/// Struct for level configuration.
struct LevelConfig {

    /// Filepath for the map file.
    std::string filePath;

    /// Starting position of the player.
    SDL_Point startPos;

    /// Exit position.
    SDL_Point exitPos;

    /// A vector of defined enemies.
    std::vector<EnemySpawnInfo> enemies;
};

#endif //LEVELCONFIG_H
