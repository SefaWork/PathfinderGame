#ifndef RANDOM_MOVEMENT_H
#define RANDOM_MOVEMENT_H
#include "../movementStrategy.h"
#include <random>

class RandomMovementStrategy : public IMovementStrategy {
public:
    RandomMovementStrategy();
    SDL_Point getNextMovement(const SDL_Point &currentPos, const SDL_Point &targetPos, const Map &map, const WavefrontMap *wavefrontMap) override;
private:
    std::mt19937 rng;
};

#endif //RANDOM_MOVEMENT_H
