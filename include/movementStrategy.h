#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <SDL_rect.h>
#include "map.h"

using WavefrontMap = std::vector<std::vector<int>>;

class Enemy; // Forward declaration of Enemy class.

/// Abstract class for defining movement strategies.
class IMovementStrategy {
public:
    virtual ~IMovementStrategy() = default;

    /// Calculates enemy's next movement.
    /// @param currentPos Enemy's current position.
    /// @param targetPos Player's position.
    /// @param map Current map.
    /// @param wavefrontMap Calculated wavefront map.
    /// @return
    virtual SDL_Point getNextMovement(const SDL_Point& currentPos, const SDL_Point& targetPos, const Map& map) = 0;
};

#endif //MOVEMENTSTRATEGY_H
