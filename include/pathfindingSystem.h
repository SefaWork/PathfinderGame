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

/// Handles game pathfinding system.
class PathfindingSystem {
public:
    PathfindingSystem();
    ~PathfindingSystem();

    /// Calculates a path from starting position to end position on map using algorithm of type.
    /// @param start Start tile position
    /// @param end End tile position
    /// @param map Current map
    /// @param type Algorithm to be used
    /// @return Path that was calculated.
    Path findPath(const SDL_Point& start, const SDL_Point& end, const Map& map, AlgorithmType type);

    /// Generates a wavefront map for chaser enemies. This is stored inside of map.
    /// @param target Target position.
    /// @param map Current map.
    /// @return Wavefront map.
    WavefrontMap generateWavefrontMap(const SDL_Point& target, const Map& map);
private:
    // Following pathfinders were abstracted to Pathfinder generic class.

    Pathfinder* bfsPathfinder;
    Pathfinder* astarPathfinder;
    Pathfinder* dijkstraPathfinder;
};

#endif //PATHFINDINGSYSTEM_H
