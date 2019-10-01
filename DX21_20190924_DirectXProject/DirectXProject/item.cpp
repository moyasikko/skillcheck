/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "item.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "debug.h"
#include "effect.h"
#include "player.h"
#include "judgement.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define ITEM_SPEED (3.0f)


/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct ITEM
{
	ITEM_TYPE type;
	D3DXVECTOR2 Position;
	D3DXVECTOR2 Speed;
	bool Flag;
	Circle Collision;
	bool collect;
};

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static ITEM g_Item[ITEM_MAX];

static int g_Texture;


void Item_Init(void)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/item_p.png", 30, 30);
	for (int i = 0; i < ITEM_MAX; i++)
	{
		g_Item[i].Flag = false;
		g_Item[i].Speed.y = -ITEM_SPEED;
		g_Item[i].Speed.x = 0.0f;
		g_Item[i].Collision.radius = 40;
		g_Item[i].collect = false;
	}
}

void Item_Uninit(void)
{
	Texture_Destroy(&g_Texture, 1);
}

void Item_Update(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!g_Item[i].Flag)
		{
			continue;
		}
		if (Player_GetPosition().y < 100)
			g_Item[i].collect = true;
		if (g_Item[i].type == ITEMTYPE1)
		{
			if (Player_GetPower() >= 500)
			{
				g_Item[i].type = ITEMTYPE2;
			}
		}
		//アイテムの座標の更新
		if (g_Item[i].collect)
		{
			g_Item[i].Speed = Player_GetPosition() - g_Item[i].Position;
			D3DXVec2Normalize(&g_Item[i].Speed, &g_Item[i].Speed);
			g_Item[i].Speed *= 20;
		}
		else
		{
			if (g_Item[i].Speed.y < ITEM_SPEED)
				g_Item[i].Speed.y += 0.02;
		}
		g_Item[i].Position += g_Item[i].Speed;
		g_Item[i].Collision.position = g_Item[i].Position;
		if (g_Item[i].Position.y > SCREEN_HEIGHT)
		{
			g_Item[i].Flag = false;
		}
	}
}

void Item_Draw(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_Item[i].Flag)
		{
			Sprite_DrawScale(g_Texture, g_Item[i].Position.x, g_Item[i].Position.y, 1.0f);
			Debug_SetColor(D3DCOLOR(0xFF0000FF));
			Debug_Draw(g_Item[i].Collision.position.x, g_Item[i].Collision.position.y, g_Item[i].Collision.radius);
		}
	}
}

void Item_Create(float x, float y, ITEM_TYPE type)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_Item[i].Flag)
		{
			continue;
		}
		g_Item[i].type = type;
		g_Item[i].Position = D3DXVECTOR2(x, y);
		g_Item[i].Collision.position = g_Item[i].Position;
		g_Item[i].Speed.y = -ITEM_SPEED;
		g_Item[i].Speed.x = 0.0f;
		g_Item[i].Flag = true;
		g_Item[i].collect = false;
		break;
	}
}

bool Item_Flag(int i)
{
	return g_Item[i].Flag;
}

void Item_Destroy(int i)
{
	g_Item[i].Flag = false;
	switch (g_Item[i].type)
	{
	case ITEMTYPE1:
		AddScore(100);
		break;
	case ITEMTYPE2:
		AddScore(1000);
		break;
	case ITEMTYPE3:
		AddScore(100);
		break;
	default:
		break;
	}
}

const Circle* Item_GetCollision(int i)
{
	return &g_Item[i].Collision;
}