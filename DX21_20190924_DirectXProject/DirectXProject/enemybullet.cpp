/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "enemybullet.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "explosion.h"
#include "player.h"
#include "debug.h"
#include "effect.h"
#include <math.h>

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define ENEMYBULLET_SPEED (3.0f)


/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct ENEMYBULLET
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
static ENEMYBULLET g_EnemyBullet[ENEMYBULLET_MAX];

static int g_Texture[3];

static int g_Frame;


void EnemyBullet_Init(void)
{
	g_Texture[0] = Texture_SetLoadFile("./sozai/img/Ebullet-A.png", 20, 20);
	g_Texture[2] = Texture_SetLoadFile("./sozai/img/Ebullet-B.png", 20, 20);
	g_Texture[3] = Texture_SetLoadFile("./sozai/img/Ebullet-C.png", 20, 20);
	for (int i = 0; i < ENEMYBULLET_MAX; i++)
	{
		g_EnemyBullet[i].Flag = false;
		g_EnemyBullet[i].Speed.x = 0.0f;
		g_EnemyBullet[i].Speed.y = 0.0f;
		g_EnemyBullet[i].Collision.radius = 20 / 2;
	}
}

void EnemyBullet_Uninit(void)
{
	Texture_Destroy(g_Texture, 3);
}

void EnemyBullet_Update(void)
{
	for (int i = 0; i < ENEMYBULLET_MAX; i++)
	{
		if (!g_EnemyBullet[i].Flag)
		{
			continue;
		}

		//弾の座標の更新
		g_EnemyBullet[i].Position += g_EnemyBullet[i].Speed;
		g_EnemyBullet[i].Collision.position = g_EnemyBullet[i].Position;
		g_EnemyBullet[i].Angle -= 5;
		//Effect_Create(g_EnemyBullet[i].Position.x, g_EnemyBullet[i].Position.y, g_EnemyBullet[i].Collision.radius * 2, D3DCOLOR_RGBA(255, 32, 128, 255), 30);
		if (g_EnemyBullet[i].Position.x < 0 ||
			g_EnemyBullet[i].Position.x > GAME_WIDTH ||
			g_EnemyBullet[i].Position.y > SCREEN_HEIGHT ||
			g_EnemyBullet[i].Position.y < 0 -200)
		{
			g_EnemyBullet[i].Flag = false;
		}
	}
	g_Frame++;
}

void EnemyBullet_Draw(void)
{
	for (int i = 0; i < ENEMYBULLET_MAX; i++)
	{
		if (g_EnemyBullet[i].Flag)
		{
			Sprite_DrawRotate(g_Texture[0], g_EnemyBullet[i].Position.x, g_EnemyBullet[i].Position.y, g_EnemyBullet[i].Angle);
			Debug_SetColor(D3DCOLOR(0xFFFF0000));
			Debug_Draw(g_EnemyBullet[i].Collision.position.x, g_EnemyBullet[i].Collision.position.y, g_EnemyBullet[i].Collision.radius);
		}
	}
}

void EnemyBullet_Create(float x, float y, ENEMY_BULLETTYPE type, int option)
{
	switch (type)
	{
	case BULLETTYPE1:
		for (int i = 0; i < ENEMYBULLET_MAX; i++)
		{
			if (g_EnemyBullet[i].Flag)
			{
				continue;
			}

			g_EnemyBullet[i].Position = D3DXVECTOR2(x, y);
			g_EnemyBullet[i].Collision.position = g_EnemyBullet[i].Position;
			g_EnemyBullet[i].Flag = true;
			g_EnemyBullet[i].Angle = 0;
			g_EnemyBullet[i].Speed = Player_GetPosition() - g_EnemyBullet[i].Position;
			D3DXVec2Normalize(&g_EnemyBullet[i].Speed, &g_EnemyBullet[i].Speed);
			g_EnemyBullet[i].Speed *= ENEMYBULLET_SPEED;
			break;
		}
		break;
	case BULLETTYPE2:
		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < ENEMYBULLET_MAX; j++)
			{
				if (g_EnemyBullet[j].Flag)
				{
					continue;
				}

				g_EnemyBullet[j].Position = D3DXVECTOR2(x, y);
				g_EnemyBullet[j].Collision.position = g_EnemyBullet[j].Position;
				g_EnemyBullet[j].Flag = true;
				g_EnemyBullet[j].Angle = option;
				g_EnemyBullet[j].Speed = D3DXVECTOR2(cos(g_EnemyBullet[j].Angle * 3.14 / 180), sin(g_EnemyBullet[j].Angle * 3.14 / 180));
				D3DXVec2Normalize(&g_EnemyBullet[j].Speed, &g_EnemyBullet[j].Speed);
				g_EnemyBullet[j].Speed *= ENEMYBULLET_SPEED;
				break;
			}
		}
		break;
	case BULLETTYPE3:
		for (int i = 0; i < ENEMYBULLET_MAX; i++)
		{
			if (g_EnemyBullet[i].Flag)
			{
				continue;
			}

			g_EnemyBullet[i].Position = D3DXVECTOR2(x, y);
			g_EnemyBullet[i].Collision.position = g_EnemyBullet[i].Position;
			g_EnemyBullet[i].Flag = true;
			g_EnemyBullet[i].Angle = option*2;
			g_EnemyBullet[i].Speed = D3DXVECTOR2(cos(g_EnemyBullet[i].Angle * 3.14 / 180), sin(g_EnemyBullet[i].Angle * 3.14 / 180));
			D3DXVec2Normalize(&g_EnemyBullet[i].Speed, &g_EnemyBullet[i].Speed);
			g_EnemyBullet[i].Speed *= ENEMYBULLET_SPEED;
			break;
		}
		break;
	case BULLETTYPE4:
		for (int i = 0; i < ENEMYBULLET_MAX; i++)
		{
			if (g_EnemyBullet[i].Flag)
			{
				continue;
			}

			g_EnemyBullet[i].Position = D3DXVECTOR2(x, y);
			g_EnemyBullet[i].Collision.position = g_EnemyBullet[i].Position;
			g_EnemyBullet[i].Flag = true;
			g_EnemyBullet[i].Angle = option * 2;
			g_EnemyBullet[i].Speed = D3DXVECTOR2(cos(g_EnemyBullet[i].Angle * 3.14 / 180), sin(g_EnemyBullet[i].Angle * 3.14 / 180));
			D3DXVec2Normalize(&g_EnemyBullet[i].Speed, &g_EnemyBullet[i].Speed);
			g_EnemyBullet[i].Speed *= ENEMYBULLET_SPEED;
			break;
		}
		break;
	default:
		break;
	}
	
}

bool EnemyBullet_Flag(int i)
{
	return g_EnemyBullet[i].Flag;
}

void EnemyBullet_Destroy(int i)
{
	g_EnemyBullet[i].Flag = false;
}

const Circle* EnemyBullet_GetCollision(int i)
{
	return &g_EnemyBullet[i].Collision;
}