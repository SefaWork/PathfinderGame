#ifndef SCORE_H
#define SCORE_H

class Score {
public:
    Score();
    void changeScore(int value);
    [[nodiscard]] unsigned int getScore() const {return score;}
private:
    unsigned int score;
};

#endif //SCORE_H
