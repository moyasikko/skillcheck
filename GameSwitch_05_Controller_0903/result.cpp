
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "score.h"
#include "controller.h"
#include "fade.h"
#include "sound.h"

#define SCORE_MAX (6)
#define TEXTURE_SIZE (512)
unsigned int g_ResultTexture;
static int score;
static int count;
static int frame;
static int number[6];
static bool flag;

void InitResult()
{
	g_ResultTexture = LoadTexture("rom:/number.tga");

	score = GetScore();
	count = 0;
	frame = 0;
	number[0] = 0;
	number[1] = 0;
	number[2] = 0;
	number[3] = 0;
	number[4] = 0;
	number[5] = 0;
	flag = false;
}


void UninitResult()
{
	UnloadTexture(g_ResultTexture);
}


void UpdateResult()
{
	if (!flag)
	{
		if (score == 0)
		{
			flag = true;
		}
		else 
		{
			number[0]++;
			number[0] %= 10;
		}
		if (frame >= 60)
		{
			number[1]++;
			number[1] %= 10;
		}
		if (frame >= 120)
		{
			if (score <= 100)
			{
				flag = true;
			}
			number[2]++;
			number[2] %= 10;
		}
		if (frame >= 180)
		{
			if (score <= 1000)
			{
				flag = true;
			}
			number[3]++;
			number[3] %= 10;
		}
		if (frame >= 240)
		{
			if (score <= 10000)
			{
				flag = true;
			}
			number[4]++;
			number[4] %= 10;
		}
		if (frame >= 300)
		{
			if (score <= 100000)
			{
				flag = true;
			}
			number[5]++;
			number[5] %= 10;
		}
		if (frame >= 360)
			flag = true;
		count = number[0] + (number[1] * 10) + (number[2] * 100) + (number[3] * 1000) + (number[4] * 10000) + (number[5] * 100000);
		if (GetControllerTrigger(NpadButton::A::Index))
		{
			flag = true;
		}
		if (flag)
		{
			count = score;
		}
		frame++;
	}
	else if (GetControllerTrigger(NpadButton::A::Index))
	{
		count = score;
		FlagFade();
		PlaySE(WSD_DECISION24);
	}
}


void DrawResult()
{
	SetTexture(g_ResultTexture);
	for (int i = 0; i < SCORE_MAX; i++)
	{
		int scorehyouzi = count;
		for (int j = 0; j < i; j++)
		{
			scorehyouzi /= 10;
		}
		scorehyouzi %= 10;
		DrawSprite(
			((SCORE_MAX - i - 3.5) * TEXTURE_SIZE / 8),
			TEXTURE_SIZE / 10,
			TEXTURE_SIZE / 5, TEXTURE_SIZE / 5,
			0.2f * (scorehyouzi % 5), 0.2f * (scorehyouzi / 5),
			0.2f, 0.2f,
			MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}