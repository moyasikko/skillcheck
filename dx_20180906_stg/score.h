#ifndef SCORE_H_
#define SCORE_H_

void Score_Initialize(int digit);
void Score_Finalize(void);
void Score_Draw(float x, float y, int score, bool bZero, bool bLeft);
void Power_Draw(float x, float y, int power, bool bZero, bool bLeft);
void Life_Draw(float x, float y, int life, bool bZero, bool bLeft);
void ScoreRanking_Draw(float x, float y, int *score, bool bZero, bool bLeft);
#endif // !SCORE_H_
