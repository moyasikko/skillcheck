/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "bullet.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "debug.h"
#include "effect.h"
#include "sound.h"
#include "player.h"
#include <math.h>

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define BULLET_SPEED (10.0f)


/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct BULLET
{
	D3DXVECTOR2 Position;
	D3DXVECTOR2 Speed;
	bool Flag;
	int Angle;
	Circle Collision;
};

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static BULLET g_Bullet[BULLET_MAX];

static int g_Texture;


void Bullet_Init(void)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/bullet-A.png", 20, 20);
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Bullet[i].Flag = false;
		g_Bullet[i].Speed.y = -BULLET_SPEED;
		g_Bullet[i].Speed.x = 0.0f;
		g_Bullet[i].Collision.radius = 20 / 2;
	}
}

void Bullet_Uninit(void)
{
	Texture_Destroy(&g_Texture, 1);
}

void Bullet_Update(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!g_Bullet[i].Flag)
		{
			continue;
		}

		//弾の座標の更新
		g_Bullet[i].Position += g_Bullet[i].Speed;
		g_Bullet[i].Collision.position = g_Bullet[i].Position;
		g_Bullet[i].Angle += 5;
		//Effect_Create(g_Bullet[i].Position.x, g_Bullet[i].Position.y, g_Bullet[i].Collision.radius * 2, D3DCOLOR_RGBA(255, 64, 128, 255), 60);

		if (g_Bullet[i].Position.x < 0 || g_Bullet[i].Position.x > GAME_WIDTH || g_Bullet[i].Position.y < 0)
		{
			g_Bullet[i].Flag = false;
		}
	}
}

void Bullet_Draw(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].Flag)
		{
			Sprite_DrawRotate(g_Texture, g_Bullet[i].Position.x, g_Bullet[i].Position.y, g_Bullet[i].Angle);
			Debug_SetColor(D3DCOLOR(0xFFFFFF00));
			Debug_Draw(g_Bullet[i].Collision.position.x, g_Bullet[i].Collision.position.y, g_Bullet[i].Collision.radius);
		}
	}
}

void Bullet_Create(float x, float y)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].Flag)
		{
			continue;
		}
		g_Bullet[i].Speed.y = -BULLET_SPEED;
		g_Bullet[i].Speed.x = 0.0f;
		g_Bullet[i].Position = D3DXVECTOR2(x+15, y);
		g_Bullet[i].Collision.position = g_Bullet[i].Position;
		g_Bullet[i].Flag = true;
		g_Bullet[i].Angle = 0;
		break;
	}
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].Flag)
		{
			continue;
		}
		g_Bullet[i].Speed.y = -BULLET_SPEED;
		g_Bullet[i].Speed.x = 0.0f;
		g_Bullet[i].Position = D3DXVECTOR2(x-15, y);
		g_Bullet[i].Collision.position = g_Bullet[i].Position;
		g_Bullet[i].Flag = true;
		g_Bullet[i].Angle = 0;
		break;
	}
	if (Player_GetPower() >= 100)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}
		
			g_Bullet[i].Speed.x = cos(-80 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-80 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x + 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}
			g_Bullet[i].Speed.x = cos(-100 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-100 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x - 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
	}
	if (Player_GetPower() >= 200)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}

			g_Bullet[i].Speed.x = cos(-70 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-70 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x + 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}
			g_Bullet[i].Speed.x = cos(-110 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-110 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x - 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
	}
	if (Player_GetPower() >= 300)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}

			g_Bullet[i].Speed.x = cos(-60 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-60 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x + 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}
			g_Bullet[i].Speed.x = cos(-120 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-120 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x - 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
	}
	if (Player_GetPower() >= 400)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}

			g_Bullet[i].Speed.x = cos(-85 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-85 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x + 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}
			g_Bullet[i].Speed.x = cos(-95 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-95 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x - 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
	}
	if (Player_GetPower() >= 500)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}

			g_Bullet[i].Speed.x = cos(-75 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-75 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x + 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].Flag)
			{
				continue;
			}
			g_Bullet[i].Speed.x = cos(-115 * (3.14 / 180));
			g_Bullet[i].Speed.y = sin(-115 * (3.14 / 180));
			g_Bullet[i].Speed *= BULLET_SPEED;
			g_Bullet[i].Position = D3DXVECTOR2(x - 15, y);
			g_Bullet[i].Collision.position = g_Bullet[i].Position;
			g_Bullet[i].Flag = true;
			g_Bullet[i].Angle = 0;
			break;
		}
	}
}

bool Bullet_Flag(int i)
{
	return g_Bullet[i].Flag;
}

void Bullet_Destroy(int i)
{
	PlaySound(SOUND_LABEL_SE_HIT);
	g_Bullet[i].Flag = false;
}

const Circle* Bullet_GetCollision(int i)
{
	return &g_Bullet[i].Collision;
}