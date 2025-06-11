#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <SDL.h>
#include <vector>
#include "map.h"

class PathfindingSystem; // Forward declaration of PathfindingSystem.

using Path = std::vector<SDL_Point>;

/// Abstract class for defining pathfinder behavior.
class Pathfinder {
public:
    virtual ~Pathfinder() = default;

    /// Abstract method for finding path.
    /// @param start start position
    /// @param end end position
    /// @param map current map
    /// @return Path
    virtual Path findPath(const SDL_Point& start, const SDL_Point& end, const Map& map) = 0;
protected:

    /// Calculates heuristic.
    int heuristic(const SDL_Point& a, const SDL_Point& b);

    /// Reconstructs path.
    Path reconstructPath(const SDL_Point& start, const SDL_Point& end, const Map& map, const std::vector<std::vector<SDL_Point>>& cameFrom);
};

#endif //PATHFINDER_H
