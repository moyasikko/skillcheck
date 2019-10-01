/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "score.h"
#include "number.h"
#include <stdio.h>

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_Digit;
static int g_CounterStopValue = 0;


void Score_Initialize(int digit)
{
	Number_Initialize();

	g_Digit = digit;

	g_CounterStopValue = 1;
	for (int i = 0; i < digit; i++)
	{
		g_CounterStopValue *= 10;
	}
	g_CounterStopValue--;
}
void Score_Finalize(void)
{
	Number_Finalize();
}
void Score_Draw(float x, float y, int score, bool bZero, bool bLeft)
{
	if (score >= g_CounterStopValue)
		score = g_CounterStopValue;
	for (int i = g_Digit - 1; i >= 0; i--)
	{
		Number_Draw(i * NUMBER_WIDTH + x, y, score % 10, 1);

		score /= 10;
	}
}
void Power_Draw(float x, float y, int power, bool bZero, bool bLeft)
{
	short digit = 3;
	/*if (power >= 10)
		digit++;
	if (power >= 100)
		digit++;*/


	if (power >= 500)
		power = 500;
	for (int i = digit - 1; i >= 0; i--)
	{
		float size;
		if (i == 0)
		{
			size = 1.0f;
		}
		else
		{
			size = 0.7f;
		}
		Number_Draw(x - digit * NUMBER_WIDTH + i * NUMBER_WIDTH + (NUMBER_WIDTH - NUMBER_WIDTH * size), y + (NUMBER_HEIGHT - NUMBER_HEIGHT * size) / 2, power % 10, size);

		power /= 10;
	}
}
void Life_Draw(float x, float y, int life, bool bZero, bool bLeft)
{
	short digit = 1;
	if (life >= 10)
	{
		//life = 10;
		digit++;
	}
	for (int i = digit - 1; i >= 0; i--)
	{
		Number_Draw(x - digit * NUMBER_WIDTH + i * NUMBER_WIDTH, y, life % 10, 1);

		life /= 10;
	}
}
void ScoreRanking_Draw(float x, float y, int score[RANKING], bool bZero, bool bLeft)
{
	int ranking_score[RANKING];
	FILE *fp;
	fp = fopen("score.txt", "rt");

	for (int i = 0; i < RANKING; i++)
	{
		fscanf(fp, "%d", &ranking_score[i]);
	}
	fclose(fp);
	for (int i = 0; i < RANKING; i++)
	{
		if (ranking_score[i] >= g_CounterStopValue)
			ranking_score[i] = g_CounterStopValue;
		for (int j = g_Digit - 1; j >= 0; j--)
		{
			Number_Draw(j * NUMBER_WIDTH + x, y + (i * NUMBER_HEIGHT), ranking_score[i] % 10, 1);

			ranking_score[i] /= 10;
		}
	}
}