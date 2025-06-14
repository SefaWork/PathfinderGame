#ifndef WAVEFRONTCHASE_H
#define WAVEFRONTCHASE_H
#include "../movementStrategy.h"

/// Subclass of IMovementStrategy that implements wavefront chaser movement.
class WavefrontChaseMovementStrategy : public IMovementStrategy {
public:
    WavefrontChaseMovementStrategy();
    SDL_Point getNextMovement(const SDL_Point &currentPos, const SDL_Point &targetPos, const Map &map) override;
};

#endif //WAVEFRONTCHASE_H
