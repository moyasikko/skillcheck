/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "debug_font.h"
#include "bullet.h"
#include "explosion.h"
#include "sound.h"
#include "debug.h"
#include "fade.h"
#include "anime.h"
#include "bomb.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define PLAYER_SPEED (10.0f)
#define MUTEKIZIKAN (180)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct PLAYER
{
	D3DXVECTOR2 Position;
	D3DXVECTOR2 Speed;
	Circle Collision;
	bool MutekiFlag;
	bool StartFlag;
	int MutekiCount;
	int BulletInterval;
	float Angle;
	int Alpha;
	int Muki;
	int Power;
	int Life;
};

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static PLAYER g_Player;
static int g_Texture;

static bool g_Fade = false;
static bool g_GamepadUse = false;
static int g_Frame;

void Player_Init(void)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/player.png", 150, 50);
	g_Player.Position.x = GAME_WIDTH / 2;
	g_Player.Position.y = SCREEN_HEIGHT + (Texture_GetHeight(g_Texture) / 2);
	g_Player.Speed.x = 0.0f;
	g_Player.Speed.y = ((SCREEN_HEIGHT - 100.0f) - g_Player.Position.y) / 60.0f;
	
	g_Player.Collision.position = g_Player.Position;
	g_Player.Collision.radius = 1;
	g_Player.MutekiFlag = true;
	g_Player.StartFlag = true;
	g_Player.MutekiCount = MUTEKIZIKAN + 60;
	g_Player.BulletInterval = 0;
	g_Player.Angle = 0.0f;
	g_Player.Alpha = 127;
	g_Player.Muki = 1;
	g_Player.Power = 0;
	g_Player.Life = 10;
	g_GamepadUse = GamePad_GetFlag();
}

void Player_Uninit(void)
{
	Texture_Destroy(&g_Texture, 1);
}
void Player_Update(void)
{
	if (g_Player.StartFlag)
	{
		g_Player.Position += g_Player.Speed;
		if (g_Frame / 2 % 2 == 0)
		{
			g_Player.Alpha = 255;
		}
		else
		{
			g_Player.Alpha = 64;
		}
		g_Player.MutekiCount--;
		if (g_Player.MutekiCount <= MUTEKIZIKAN)
		{
			g_Player.StartFlag = false;
		}
	}
	else
	{
		g_Player.Speed *= 0;
		
		if (g_GamepadUse)
		{
			g_Player.Speed = GamePad_GetThumbL();
		}
		else
		{
			if (Keyboard_IsPress(DIK_D))
			{
				g_Player.Speed.x = 1.0f;
			}
			if (Keyboard_IsPress(DIK_A))
			{
				g_Player.Speed.x = -1.0f;
			}
			if (Keyboard_IsPress(DIK_W))
			{
				g_Player.Speed.y = -1.0f;
			}
			if (Keyboard_IsPress(DIK_S))
			{
				g_Player.Speed.y = 1.0f;
			}
			D3DXVec2Normalize(&g_Player.Speed, &g_Player.Speed);
		}
		
		if (g_Player.Speed.x > 0)
		{
			g_Player.Muki = 0;
		}
		else if (g_Player.Speed.x < 0)
		{
			g_Player.Muki = 2;
		}
		else
		{
			g_Player.Muki = 1;
		}
		if (Keyboard_IsPress(DIK_M) || GamePad_IsPress(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			g_Player.Speed /= 2;
		}
		g_Player.Position += g_Player.Speed * PLAYER_SPEED;

		if (g_Player.Position.x > GAME_WIDTH - 25)
			g_Player.Position.x = GAME_WIDTH - 25;
		if (g_Player.Position.x < 25)
			g_Player.Position.x = 25;
		if (g_Player.Position.y > SCREEN_HEIGHT - 25)
			g_Player.Position.y = SCREEN_HEIGHT - 25;
		if (g_Player.Position.y < 25)
			g_Player.Position.y = 25;

		if (Keyboard_IsPress(DIK_SPACE) || GamePad_IsPress(XINPUT_GAMEPAD_A))
		{
			if (g_Player.BulletInterval <= 0)
			{
				Bullet_Create(g_Player.Position.x, g_Player.Position.y);
				PlaySound(SOUND_LABEL_SE_SHOT);
				g_Player.BulletInterval = 5;
			}
		}
		if (Keyboard_IsRelease(DIK_SPACE) || GamePad_IsRelease(XINPUT_GAMEPAD_A))
		{
			g_Player.BulletInterval = 0;
		}
		if (!Bomb_Flag() && (Keyboard_IsTrigger(DIK_B) || GamePad_IsTrigger(XINPUT_GAMEPAD_X)) && g_Player.Power >= 100)
		{
			Bomb_Create(g_Player.Position.x, g_Player.Position.y);
			g_Player.Power -= 100;
		}
		g_Player.BulletInterval--;
		if (g_Player.MutekiFlag)
		{
			if (g_Frame / 2 % 2 == 0)
			{
				g_Player.Alpha = 255;
			}
			else
			{
				g_Player.Alpha = 64;
			}
			g_Player.MutekiCount--;
			if (g_Player.MutekiCount <= 0)
			{
				g_Player.MutekiFlag = false;
				g_Player.Alpha = 255;
			}
		}
	}
	g_Player.Collision.position = g_Player.Position;
	g_Frame++;
}
void Player_Draw(void)
{
	Sprite_DrawCut(g_Texture, g_Player.Position.x, g_Player.Position.y, g_Player.Muki * 50, 0, 50, 50, D3DCOLOR_RGBA(255, 255, 255, g_Player.Alpha));

	#if defined(_DEBUG) || defined(DEBUG)
		DebugFont_Draw(0, 40, "PLAYER_LIFE:%d", g_Player.Life);
		
		Debug_SetColor(D3DCOLOR(0xFFFFFFFF));
		Debug_Draw(g_Player.Collision.position.x, g_Player.Collision.position.y, g_Player.Collision.radius);
	#endif // _DEBUG || DEBUG
}

const Circle* Player_GetCollision(void)
{
	return &g_Player.Collision;
}

void Player_Damege(void)
{
	g_Player.MutekiFlag = true;
	g_Player.MutekiCount = MUTEKIZIKAN;
	g_Player.Alpha = 127;
	Anime_Create(g_Player.Position.x, g_Player.Position.y);
	g_Player.Life--;
}

bool Player_GetFlag(void)
{
	return g_Player.MutekiFlag;
}

D3DXVECTOR2 Player_GetPosition(void)
{
	return g_Player.Position;
}

void Player_PowerUp(void)
{
	g_Player.Power += 5;
	if (g_Player.Power % 100 == 0)
	{
		PlaySound(SOUND_LABEL_SE_POWERUP);
	}
	if (g_Player.Power > 500)
	{
		g_Player.Power = 500;
	}
}

int Player_GetPower(void)
{
	return g_Player.Power;
}