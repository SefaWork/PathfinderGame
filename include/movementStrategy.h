#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <SDL_rect.h>
#include "map.h"

using WavefrontMap = std::vector<std::vector<int>>;

class Enemy; // Forward declaration of Enemy class.

class IMovementStrategy {
public:
    virtual ~IMovementStrategy() = default;
    virtual SDL_Point getNextMovement(const SDL_Point& currentPos, const SDL_Point& targetPos, const Map& map, const WavefrontMap* wavefrontMap) = 0;
};

#endif //MOVEMENTSTRATEGY_H
