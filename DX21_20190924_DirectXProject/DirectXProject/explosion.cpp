/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "texture.h"
#include "sprite.h"
#include "sound.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define EXPLOSION_MAX (32)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct EXPLOSION
{
	D3DXVECTOR2 Position;
	bool Flag;
	int count;
};

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_Texture;
static EXPLOSION g_Explosion[EXPLOSION_MAX];


void Explosion_Init(void)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/explosion.png", 960, 960);
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		g_Explosion[i].Position = D3DXVECTOR2(0, 0);
		g_Explosion[i].Flag = false;
		g_Explosion[i].count = 0;
	}
}
void Explosion_Uninit(void)
{
	Texture_Destroy(&g_Texture, 1);
}
void Explosion_Update(void)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		g_Explosion[i].count++;
		if (g_Explosion[i].count >= 50)
		{
			g_Explosion[i].Flag = false;
		}
	}
}
void Explosion_Draw(void)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (g_Explosion[i].Flag)
		{
			Sprite_DrawCut(g_Texture, g_Explosion[i].Position.x, g_Explosion[i].Position.y, ((g_Explosion[i].count / 5) % 5) * 192, ((g_Explosion[i].count / 5) / 5) * 192, 192, 192);
		}
	}
}

void Explosion_Create(float x, float y)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_Explosion[i].Flag)
		{
			g_Explosion[i].Position.x = x;
			g_Explosion[i].Position.y = y;
			g_Explosion[i].Flag = true;
			g_Explosion[i].count = 0;
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			break;
		}
	}
}