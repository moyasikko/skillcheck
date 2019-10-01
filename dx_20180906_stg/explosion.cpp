/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "explosion.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define EXPLOSION_SIZE	(192)
#define EXPLOSION_MAX	(256)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_TextureIndex = { TEXTURE_INVALID_INDEX };
static int anime_x[EXPLOSION_MAX];
static int anime_y[EXPLOSION_MAX];
static D3DXVECTOR2 g_Position[EXPLOSION_MAX];
static bool flag[EXPLOSION_MAX];

void Explosion_Initialize(void)
{
	g_TextureIndex = Texture_SetLoadFile("./img/explosion.png", EXPLOSION_SIZE * 5, EXPLOSION_SIZE * 5);
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		flag[i] = false;
		anime_x[i] = anime_y[i] = 0.0f;
		g_Position[i].x = 0.0f;
		g_Position[i].y = 0.0f;
	}
}
void Explosion_Finalize(void)
{
	Texture_Release(&g_TextureIndex, 1);
}
void Explosion_Update(void)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (flag[i])
		{
			anime_x[i]++;
			if (anime_x[i] >= 10)
			{
				anime_x[i] = 0;
				anime_y[i]++;
			}
			if (anime_y[i] == 3 && anime_x[i] == 6)
			{
				anime_x[i] = anime_y[i] = 0;
				flag[i] = false;
			}
		}
	}
}
void Explosion_Draw()
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	if (flag[i])
	{
		Sprite_Draw(g_TextureIndex, g_Position[i].x - EXPLOSION_SIZE /2, g_Position[i].y - EXPLOSION_SIZE / 2, (anime_x[i] / 2) *EXPLOSION_SIZE, anime_y[i] * EXPLOSION_SIZE, EXPLOSION_SIZE, EXPLOSION_SIZE);
	}
}

void Explosion_Create(D3DXVECTOR2 position)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (flag[i] == false)
		{
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			flag[i] = true;
			g_Position[i] = position;
			break;
		}
	}
}