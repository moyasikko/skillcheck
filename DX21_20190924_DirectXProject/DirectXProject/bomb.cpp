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

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define BOMB_SPEED (10.0f)


/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct BOMB
{
	D3DXVECTOR2 Position;
	bool Flag;
	int Angle;
	int Age;
	Circle Collision;
};

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static BOMB g_Bomb;


void Bomb_Init(void)
{
	g_Bomb.Flag = false;
	g_Bomb.Collision.radius = 2;

}

void Bomb_Uninit(void)
{
	
}

void Bomb_Update(void)
{
	if (!g_Bomb.Flag)
	{
		return;
	}

	//ボムの更新
	g_Bomb.Collision.radius = g_Bomb.Age * 3;
	g_Bomb.Angle += 5;
	Effect_Create(	g_Bomb.Position.x + ((rand() % (int)(g_Bomb.Collision.radius * 1.5f)) - g_Bomb.Collision.radius * 0.75f),
					g_Bomb.Position.y + ((rand() % (int)(g_Bomb.Collision.radius * 1.5f)) - g_Bomb.Collision.radius * 0.75f),
					g_Bomb.Collision.radius,
					D3DCOLOR_RGBA(128, 128, 32, 255), 60);

	if (g_Bomb.Age > 100)
	{
		g_Bomb.Flag = false;
	}
	g_Bomb.Age++;
}

void Bomb_Draw(void)
{
		if (g_Bomb.Flag)
		{
			Debug_SetColor(D3DCOLOR(0xFFFFFF00));
			Debug_Draw(g_Bomb.Collision.position.x, g_Bomb.Collision.position.y, g_Bomb.Collision.radius);
		}
}

void Bomb_Create(float x, float y)
{
		g_Bomb.Position = D3DXVECTOR2(x, y);
		g_Bomb.Collision.position = g_Bomb.Position;
		g_Bomb.Collision.radius = 2;
		g_Bomb.Flag = true;
		g_Bomb.Angle = 0;
		g_Bomb.Age = 1;
}

bool Bomb_Flag(void)
{
	return g_Bomb.Flag;
}

void Bomb_Destroy(void)
{
	g_Bomb.Flag = false;
}

const Circle* Bomb_GetCollision(void)
{
	return &g_Bomb.Collision;
}