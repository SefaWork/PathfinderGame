#ifndef BFSPATHFINDER_H
#define BFSPATHFINDER_H
#include "../pathfinder.h"

class BFSPathfinder : public Pathfinder {
public:
    BFSPathfinder() = default;
    Path findPath(const SDL_Point &start, const SDL_Point &end, const Map &map) override;
};

#endif //BFSPATHFINDER_H
