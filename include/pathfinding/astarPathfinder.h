#ifndef ASTARPATHFINDER_H
#define ASTARPATHFINDER_H
#include "../pathfinder.h"

class AStarPathfinder : public Pathfinder {
public:
    AStarPathfinder() = default;
    Path findPath(const SDL_Point &start, const SDL_Point &end, const Map &map) override;
};

#endif //ASTARPATHFINDER_H
