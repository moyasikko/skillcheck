
#include "main.h"
#include "controller.h"
#include "texture.h"
#include "sprite.h"
#include "block.h"
#include "player.h"
#include "fade.h"
#include "sound.h"


unsigned int	g_BallTexture;
struct BALL
{
	Float2			Position;
	Float2			Speed;
	Float2			Speed2;
	float			kasoku;
}g_Ball;
bool flag = false;
int hantei;
int life;
bool reset = false;

void InitBall()
{
	life = 3;
	g_BallTexture = LoadTexture("rom:/ball.tga");

	g_Ball.Position.x = 0.0f;
	g_Ball.Position.y = 0.0f;
	g_Ball.Speed.x = 5.0f;
	g_Ball.Speed.y = 9.0f;
	g_Ball.kasoku = 10.0f;
}



void UninitBall()
{

	UnloadTexture(g_BallTexture);

}


void UpdateBall()
{
	if (!reset)
	{
		g_Ball.Position.y += g_Ball.Speed.y;
		hantei = CollisionBlock(g_Ball.Position);
		if (hantei == 2)
		{
			if (!flag)
			{
				g_Ball.Speed.y *= -1;
				PlaySE(WSD_WATER_DROP2);
			}
			flag = true;
		}

		hantei = CollisionPlayer(g_Ball.Position);
		if (hantei == 2)
		{
			if (!flag)
			{
				g_Ball.Speed.y *= -1;
				PlaySE(WSD_WATER_DROP2);
			}
			flag = true;
			g_Ball.Position.x += g_Ball.Speed.x;

		}
		else
		{
			g_Ball.Position.x += g_Ball.Speed.x;
			hantei = CollisionBlock(g_Ball.Position);
			if (hantei == 2)
			{
				if (!flag)
				{
					g_Ball.Speed.x *= -1;
					PlaySE(WSD_WATER_DROP2);
				}
				flag = true;
			}


			hantei = CollisionPlayer(g_Ball.Position);
			if (hantei == 2)
			{
				if (!flag)
				{
					g_Ball.Speed.x *= -1;
					g_Ball.Speed.y *= -1;
					PlaySE(WSD_WATER_DROP2);
				}
				flag = true;
			}
			else
			{
				flag = false;
			}
		}
	}
	if (flag)
	{
		g_Ball.Speed.x *= 1.0f + ((4.0f - (float)life) / 300.0f);
		g_Ball.Speed.y *= 1.0f + ((4.0f - (float)life) / 300.0f);
	}

	if (reset)
	{
		g_Ball.Position = PositionPlayer();
		g_Ball.Position.y -= 48.0f;
	}

	if (g_Ball.Position.x < -500.0f + 30.0f)
	{
		g_Ball.Speed.x *= -1;
		PlaySE(WSD_WATER_DROP2);
	}

	if (g_Ball.Position.x > 500.0f - 30.0f)
	{
		g_Ball.Speed.x *= -1;
		PlaySE(WSD_WATER_DROP2);
	}

	if (g_Ball.Position.y < -500.0f + 30.0f)
		if (g_Ball.Speed.y < 0)
		{
			g_Ball.Speed.y *= -1;
			PlaySE(WSD_WATER_DROP2);
		}

	if (g_Ball.Position.y > 800.0f)
	{
		life--;
		g_Ball.Speed.x = 5.0f;
		g_Ball.Speed.y = -9.0f;
		if(life < 0)
			FlagFade();
		else
		{
			reset = true;
		}
	}
	if (reset && GetControllerTrigger(NpadButton::A::Index))
	{
		//g_Ball.Speed.x = 5.0f;
		//g_Ball.Speed.y = -9.0f;
		reset = false;
	}
	if (GetFlagBlock())
	{
		reset = true;
		ReBlock();
	}
}



void DrawBall()
{

	SetTexture(g_BallTexture);

	DrawSprite(g_Ball.Position.x, g_Ball.Position.y, 64.0f, 64.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f));
	for (int i = 0; i < life; i++)
	{
		DrawSprite(600.0f, -SCREEN_HEIGHT / 2 + (80 * (i + 1)), 64.0f, 64.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f));
	}

}

float GetPositionBall()
{
	return g_Ball.Position.x;
}