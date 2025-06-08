#ifndef PATHFINDINGSYSTEM_H
#define PATHFINDINGSYSTEM_H

#include <SDL_rect.h>
#include <vector>
#include "map.h"

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

    Path findPath(const SDL_Point& start, const SDL_Point& end, const Map& map, AlgorithmType type);
    WavefrontMap generateWavefrontMap(const SDL_Point& target, const Map& map);
private:
    Path findPathBFS(const SDL_Point& start, const SDL_Point& end, const Map& map);
    Path findPathDijkstra(const SDL_Point& start, const SDL_Point& end, const Map& map);
    Path findPathAStar(const SDL_Point& start, const SDL_Point& end, const Map& map);

    int heuristic(const SDL_Point& a, const SDL_Point& b);
    Path reconstructPath(const SDL_Point& start, const SDL_Point& end,
                         const Map& map,
                         const std::vector<std::vector<SDL_Point>>& cameFrom);
};

#endif //PATHFINDINGSYSTEM_H
