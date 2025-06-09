#ifndef PATHFINDINGSYSTEM_H
#define PATHFINDINGSYSTEM_H

#include <SDL_rect.h>
#include <vector>
#include "map.h"
#include "pathfinder.h"

using Path = std::vector<SDL_Point>;
using WavefrontMap = std::vector<std::vector<int>>;

enum class AlgorithmType {
    BFS,
    DIJKSTRA,
    ASTAR,
    WAVEFRONT
};

class PathfindingSystem {
public:
    PathfindingSystem();
    ~PathfindingSystem();

    Path findPath(const SDL_Point& start, const SDL_Point& end, const Map& map, AlgorithmType type);
    WavefrontMap generateWavefrontMap(const SDL_Point& target, const Map& map);
private:
    Pathfinder* bfsPathfinder;
    Pathfinder* astarPathfinder;
    Pathfinder* dijkstraPathfinder;
};

#endif //PATHFINDINGSYSTEM_H
