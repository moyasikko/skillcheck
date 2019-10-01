
#include "main.h"
#include "controller.h"
#include "texture.h"
#include "sprite.h"
#include "ball.h"
#include "block.h"
#include "sound.h"

unsigned int	g_PlayerTexture;
Float2			g_PlayerPosition;
Float2			g_PlayerPosition_old;
static int count;
static bool flag;


void InitPlayer()
{
	PlayBGM(STRM_NIGHT_DRIVE);
	g_PlayerTexture = LoadTexture("rom:/player.tga");

	g_PlayerPosition.x = 0.0f;
	g_PlayerPosition.y = 450.0f;
	g_PlayerPosition_old = g_PlayerPosition;
	count = 0;
	flag = false;
}



void UninitPlayer()
{

	UnloadTexture(g_PlayerTexture);
	StopBGM();
}


void UpdatePlayer()
{

	// ボタン
	if (GetControllerPress(NpadButton::Left::Index))
		g_PlayerPosition.x += -20.0f;
	if (GetControllerPress(NpadButton::Right::Index))
		g_PlayerPosition.x += 20.0f;



	// アナログスティック
	Float2 stick;
	stick = GetControllerLeftStick();

	g_PlayerPosition.x += stick.x * 20.0f;
	g_PlayerPosition.y -= stick.y * 20.0f;



	// タッチスクリーン
	if (GetControllerTouchScreen())
	{
		Float2 touch;
		touch = GetControllerTouchScreenPosition();
		g_PlayerPosition = touch;
	}



	// 加速度センサー
	/*if (GetControllerPress(NpadButton::A::Index))
	{
		Float3 acc;
		acc = GetControllerRightAcceleration();

		g_PlayerPosition.x += acc.x * 10.0f;
		g_PlayerPosition.y += -acc.y * 10.0f;
	}



	// ジャイロセンサー
	if (GetControllerPress(NpadButton::B::Index))
	{
		Float3 angle;
		angle = GetControllerRightAngle();

		g_PlayerPosition.x = angle.z * 1000.0f;
	}*/



	// 振動
	if (GetControllerPress(NpadButton::Y::Index))
	{
		SetControllerLeftVibration(5);
	}
	if (GetControllerPress(NpadButton::X::Index))
	{
		SetControllerRightVibration(5);
	}




	

	if (g_PlayerPosition.x == g_PlayerPosition_old.x &&
		g_PlayerPosition.y == g_PlayerPosition_old.y)
	{
		count++;
		if (count >= 600)
			flag = true;
	}
	else
	{
		count = 0;
		flag = false;
	}

	if (flag && !GetFlagBlock())
	{
		g_PlayerPosition.x = GetPositionBall();
	}

	if (GetFlagBlock())
	{
		g_PlayerPosition.x = 0.0f;
		g_PlayerPosition.y = 450.0f;
		g_PlayerPosition_old = g_PlayerPosition;
		count = 0;
		flag = false;
	}

	if (g_PlayerPosition.x < -500.0f + 100.0f)
		g_PlayerPosition.x = -500.0f + 100.0f;

	if (g_PlayerPosition.x > 500.0f - 100.0f)
		g_PlayerPosition.x = 500.0f - 100.0f;

	if (g_PlayerPosition.y < -500.0f + 25.0f)
		g_PlayerPosition.y = -500.0f + 25.0f;

	if (g_PlayerPosition.y > 500.0f - 0.0f)
		g_PlayerPosition.y = 500.0f - 0.0f;

	g_PlayerPosition_old = g_PlayerPosition;
}



void DrawPlayer()
{

	SetTexture(g_PlayerTexture);

	DrawSprite(g_PlayerPosition.x, g_PlayerPosition.y, 256.0f, 64.0f,
						0.0f, 0.0f, 1.0f, 1.0f,
						MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f));

}

int CollisionPlayer(Float2 ball_position)
{
	int hantei = 0;

	if ((g_PlayerPosition.y +  64.0f / 2) >= ball_position.y - 15 &&
		(g_PlayerPosition.y -  64.0f / 2) <= ball_position.y + 15 &&
		(g_PlayerPosition.x + 256.0f / 2) >= ball_position.x - 15 &&
		(g_PlayerPosition.x - 256.0f / 2) <= ball_position.x + 15)
	{
		hantei = 2;
	}


	return hantei;
}

Float2 PositionPlayer()
{
	return g_PlayerPosition;
}

Float2 PositionOldPlayer()
{
	return g_PlayerPosition_old;
}