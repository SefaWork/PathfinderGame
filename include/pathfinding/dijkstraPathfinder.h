#ifndef DIJKSTRAPATHFINDER_H
#define DIJKSTRAPATHFINDER_H
#include "../pathfinder.h"

class DijkstraPathfinder : public Pathfinder {
public:
    DijkstraPathfinder() = default;
    Path findPath(const SDL_Point &start, const SDL_Point &end, const Map &map) override;
};

#endif //DIJKSTRAPATHFINDER_H
