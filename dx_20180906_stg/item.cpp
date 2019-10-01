/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3dx9.h>
#include "item.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "player.h"


/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define ITEM_MAX		(256)
#define ITEM_SIZE		(40)
#define ITEM_SPEED		(5)

enum {
	ITEMTYPE_POWER = 0,
	ITEMTYPE_SCORE,
	ITEMTYPE_LIFE,
	ITEMTYPE_MAX
};



/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_ItemTextureIndex[ITEMTYPE_MAX] = { TEXTURE_INVALID_INDEX };
static bool Item_flag[ITEM_MAX];
static D3DXVECTOR2 g_Position[ITEM_MAX];
static D3DXVECTOR2 g_Speed[ITEM_MAX];
static Circle g_ItemCollision[ITEM_MAX];
static short Item_type[ITEM_MAX];
static bool Item_Getflag[ITEM_MAX];


void Item_Initialize(void)
{
	//テクスチャの読込予約
	g_ItemTextureIndex[ITEMTYPE_POWER] = Texture_SetLoadFile("./img/Item_P.png", ITEM_SIZE, ITEM_SIZE);
	g_ItemTextureIndex[ITEMTYPE_SCORE] = Texture_SetLoadFile("./img/Item_S.png", ITEM_SIZE, ITEM_SIZE);
	g_ItemTextureIndex[ITEMTYPE_LIFE] = Texture_SetLoadFile("./img/Item_L.png", ITEM_SIZE, ITEM_SIZE);

	//弾を無効にしておく
	for (int i = 0; i < ITEM_MAX; i++)
	{
		Item_flag[i] = false;
		Item_Getflag[i] = false;
		g_ItemCollision[i].cx = 0.0f;
		g_ItemCollision[i].cy = 0.0f;
		g_ItemCollision[i].radius = ITEM_SIZE;
	}
}
void Item_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_ItemTextureIndex[0], ITEMTYPE_MAX);
}
void Item_Update(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		//弾が無効だったら何もしない

		//弾が有効だったら
		//座標を更新する
		if (Item_flag[i])
		{
			g_Speed[i].y += 0.05;
			if (Item_Getflag[i])
			{
				g_Speed[i] = Player_GetPosition() - g_Position[i];
				D3DXVec2Normalize(&g_Speed[i], &g_Speed[i]);
				g_Speed[i] *= ITEM_SPEED * 5;
			}
			g_Position[i].x += g_Speed[i].x;
			g_Position[i].y += g_Speed[i].y;
			g_ItemCollision[i].cx = g_Position[i].x;
			g_ItemCollision[i].cy = g_Position[i].y;
			if (Item_type[i] == 0)
				if (Player_Power() >= 500)
					Item_type[i] = 1;
		}

		//弾が画面外だったら
		//無効にする
		if (g_Position[i].y > SCREEN_HEIGHT)
		{
			Item_flag[i] = false;
		}
	}
}
void Item_Draw(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		//弾が有効だったら
		//弾の絵を描く
		if (Item_flag[i])
		{
			Sprite_DrawRotate(g_ItemTextureIndex[Item_type[i]], g_Position[i].x, g_Position[i].y, 0);
		}
	}
}

void Item_Create(float x, float y, int type)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		//弾が無効だったら
		//弾を有効にして
		//弾の座標をx,yにする
		if (Item_flag[i] == false)
		{
			Item_flag[i] = true;
			Item_Getflag[i] = false;
			Item_type[i] = type;
			g_Position[i] = D3DXVECTOR2(x, y);
			g_ItemCollision[i].cx = g_Position[i].x;
			g_ItemCollision[i].cy = g_Position[i].y;
			g_Speed[i] = D3DXVECTOR2(0, -ITEM_SPEED);
			break;
		}
	}
}

void Item_Destroy(int i)
{
	Item_flag[i] = false;
	
}
bool Item_IsEnable(int i)
{
	return Item_flag[i];
}
const Circle* Item_GetCollision(int i)
{
	return &g_ItemCollision[i];
}

int Item_MaxGet(void)
{
	return ITEM_MAX;
}
int Item_TypeGet(int i)
{
	return Item_type[i];
}
void Item_GetFlag(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (Item_flag[i])
		{
			Item_Getflag[i] = true;
		}
	}
}