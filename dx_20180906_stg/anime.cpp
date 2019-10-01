/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "anime.h"
#include "texture.h"
#include "sprite.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define ANIME_SIZE	(96)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_TextureIndex = { TEXTURE_INVALID_INDEX };
static int anime_x;
static int anime_y;
static D3DXVECTOR2 g_Position(0.0f, 0.0f);
static bool flag;

void Anime_Initialize(void)
{
	g_TextureIndex = Texture_SetLoadFile("./img/blood.png", ANIME_SIZE * 5, ANIME_SIZE * 5);
	flag = false;
	anime_x = anime_y = 0;
}
void Anime_Finalize(void)
{
	Texture_Release(&g_TextureIndex, 1);
}
void Anime_Update(void)
{
	if (flag)
	{
		anime_x++;
		if (anime_x >= 10)
		{
			anime_x = 0;
			anime_y++;
		}
		if (anime_y == 2 && anime_x == 2)
		{
			anime_x = anime_y = 0;
			flag = false;
		}
	}
}
void Anime_Draw()
{
	if (flag)
	{
		Sprite_Draw(g_TextureIndex, g_Position.x - ANIME_SIZE, g_Position.y - ANIME_SIZE / 2, (anime_x / 2) * ANIME_SIZE, anime_y * ANIME_SIZE, ANIME_SIZE, ANIME_SIZE);
	}
}

void Anime_Create(D3DXVECTOR2 position)
{
	flag = true;
	g_Position = position;
}