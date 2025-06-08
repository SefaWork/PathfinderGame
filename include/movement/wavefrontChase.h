#ifndef WAVEFRONTCHASE_H
#define WAVEFRONTCHASE_H
#include "../movementStrategy.h"

class WavefrontChaseMovementStrategy : public IMovementStrategy {
public:
    WavefrontChaseMovementStrategy();
    SDL_Point getNextMovement(const SDL_Point &currentPos, const SDL_Point &targetPos, const Map &map, const WavefrontMap *wavefrontMap) override;
};

#endif //WAVEFRONTCHASE_H
