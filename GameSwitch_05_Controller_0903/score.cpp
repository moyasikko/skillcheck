
#include "main.h"
#include "texture.h"
#include "sprite.h"

#define SCORE_MAX (6)
#define TEXTURE_SIZE (512)
unsigned int g_ScoreTexture;
static int score;

void InitScore()
{
	g_ScoreTexture = LoadTexture("rom:/number.tga");

	score = 0;
}


void UninitScore()
{
	UnloadTexture(g_ScoreTexture);
}


void UpdateScore()
{
	if (score > 999999)
		score = 999999;
}


void DrawScore()
{
	SetTexture(g_ScoreTexture);
	for (int i = 0; i < SCORE_MAX; i++)
	{
		int scorehyouzi = score;
		for (int j = 0; j < i; j++)
		{
			scorehyouzi /= 10;
		}
		scorehyouzi %= 10;
		DrawSprite(
			(-SCREEN_WIDTH / 2) + (TEXTURE_SIZE / 20) + ((SCORE_MAX - i - 1) * TEXTURE_SIZE / 8),
			-SCREEN_HEIGHT / 2 + TEXTURE_SIZE / 10,
			TEXTURE_SIZE/5, TEXTURE_SIZE/5,
			0.2f * (scorehyouzi % 5), 0.2f * (scorehyouzi / 5),
			0.2f, 0.2f,
			MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void CountScore()
{
	score += 100;
}

int GetScore()
{
	return score;
}