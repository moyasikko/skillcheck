/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "option.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "debug.h"
#include "effect.h"
#include "player.h"
#include <math.h>

/*******************************************************************************
* マクロ定義
*******************************************************************************/



/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct OPTION
{
	D3DXVECTOR2 Position;
	int Age;
};

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static OPTION g_Option[OPTION_MAX];

static int g_Texture;

static int g_Frame;


void Option_Init(void)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/option.png", 64, 32);
	for (int i = 0; i < OPTION_MAX; i++)
	{
		g_Option[i].Age = 0;
	}
	g_Frame = 0;
}

void Option_Uninit(void)
{
	Texture_Destroy(&g_Texture, 1);
}

void Option_Update(void)
{
	for (int i = 0; i < OPTION_MAX; i++)
	{
		if (Player_GetPower() / 100 <= i)
		{
			continue;
		}

		//オプションの座標の更新
		g_Option[i].Position = Player_GetPosition();
		g_Option[i].Position.x += cos((g_Frame * 3 + (i * (360 / (Player_GetPower() / 100)))) * 3.14 / 180) * 50;
		g_Option[i].Position.y += sin((g_Frame * 3 + (i * (360 / (Player_GetPower() / 100)))) * 3.14 / 180) * 50;

		g_Option[i].Age ++;
	}
	g_Frame++;
}

void Option_Draw(void)
{
	for (int i = 0; i < OPTION_MAX; i++)
	{
		if (Player_GetPower() / 100 <= i)
		{
			continue;
		}
		Sprite_DrawCut(g_Texture, g_Option[i].Position.x, g_Option[i].Position.y, (g_Option[i].Age / 3) * 32, 0, 32, 32);
	}
}
