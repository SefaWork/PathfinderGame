#include "../include/score.h"

#include <cmath>

Score::Score() {
    this->score = 0;
    this->deaths = 0;
}

void Score::changeScore(int value) {
    if (value < 0 && -value > this->score) {
        // If subtracted value is higher than our total score, set to 0.
        this->score = 0;
    } else {
        // Otherwise, proceed as usual.
        this->score += value;
    }
}

void Score::incrementDeath(int scorePenalty) {
    this->deaths++;
    this->changeScore(-scorePenalty);
}


