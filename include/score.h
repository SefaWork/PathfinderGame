#ifndef SCORE_H
#define SCORE_H

class Score {
public:
    Score();
    void changeScore(int value);
    void incrementDeath(int scorePenalty);
    [[nodiscard]] unsigned int getScore() const {return score;}
    [[nodiscard]] unsigned short getDeaths() const {return deaths;}
private:
    unsigned int score;
    unsigned short deaths;
};

#endif //SCORE_H
