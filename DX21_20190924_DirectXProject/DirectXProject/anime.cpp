/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "player.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define ANIME_MAX (1)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct ANIME
{
	D3DXVECTOR2 Position;
	bool Flag;
	int count;
	bool hidari;
};

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_Texture;
static ANIME g_Anime[ANIME_MAX];


void Anime_Init(void)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/blood.png", 480, 480);
	for (int i = 0; i < ANIME_MAX; i++)
	{
		g_Anime[i].Position = D3DXVECTOR2(0, 0);
		g_Anime[i].Flag = false;
		g_Anime[i].count = 0;
	}
}
void Anime_Uninit(void)
{
	Texture_Destroy(&g_Texture, 1);
}
void Anime_Update(void)
{
	for (int i = 0; i < ANIME_MAX; i++)
	{
		g_Anime[i].Position = Player_GetPosition();
		g_Anime[i].count++;
		if (g_Anime[i].count >= 55)
		{
			g_Anime[i].Flag = false;
		}
	}
}
void Anime_Draw(void)
{
	for (int i = 0; i < ANIME_MAX; i++)
	{
		if (g_Anime[i].Flag)
		{
			if(g_Anime[i].hidari)
				Sprite_DrawCut(g_Texture, g_Anime[i].Position.x - 64, g_Anime[i].Position.y, ((g_Anime[i].count / 5) % 5) * 96, ((g_Anime[i].count / 5) / 5) * 96, 96, 96);
			else
				Sprite_DrawReverse(g_Texture, g_Anime[i].Position.x + 64, g_Anime[i].Position.y, ((g_Anime[i].count / 5) % 5) * 96, ((g_Anime[i].count / 5) / 5) * 96, 96, 96);
		}
	}
}

void Anime_Create(float x, float y)
{
	for (int i = 0; i < ANIME_MAX; i++)
	{
		if (!g_Anime[i].Flag)
		{
			g_Anime[i].Position.x = x;
			g_Anime[i].Position.y = y;
			g_Anime[i].Flag = true;
			g_Anime[i].count = 0;
			if (rand() % 2 == 0)
			{
				g_Anime[i].hidari = true;
			}
			else
			{
				g_Anime[i].hidari = false;
			}
			PlaySound(SOUND_LABEL_SE_ANIME);
			break;
		}
	}
}