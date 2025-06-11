#ifndef SCORE_H
#define SCORE_H

class Score {
public:
    Score();

    /// Changes current score.
    /// @param value Use negative value to decrement the score.
    void changeScore(int value);

    /// Increases death counter and applies score penalty.
    /// @param scorePenalty Score penalty to be applied. Do not use negative values.
    void incrementDeath(int scorePenalty);
    [[nodiscard]] unsigned int getScore() const {return score;}
    [[nodiscard]] unsigned short getDeaths() const {return deaths;}
private:
    unsigned int score;
    unsigned short deaths;
};

#endif //SCORE_H
