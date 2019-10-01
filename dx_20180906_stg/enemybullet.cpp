/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "enemybullet.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "debug_primitive.h"
#include "player.h"
#include <d3dx9.h>
#include "effect.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define EBULLET_MAX			(1024)
#define EBULLET_SIZE		(20)
#define EBULLET_SPEED		(5)
#define EBULLET_TYPE		(10)
#define PI					(3.141592)


/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_EBulletTextureIndex[EBULLET_TYPE] = { TEXTURE_INVALID_INDEX };
static bool EBullet_flag[EBULLET_MAX];
static D3DXVECTOR2 g_Position[EBULLET_MAX];
static D3DXVECTOR2 g_Speed[EBULLET_MAX];
static float angle[EBULLET_MAX];
static Circle g_EBulletCollision[EBULLET_MAX];
static short EBulletType[EBULLET_MAX];
static int EBulletLife[EBULLET_MAX];
static bool EBulletGravity[EBULLET_MAX];

void EBullet_Initialize(void)
{
	//テクスチャの読込予約
	g_EBulletTextureIndex[0] = Texture_SetLoadFile("./img/Ebullet-A.png", EBULLET_SIZE, EBULLET_SIZE);
	g_EBulletTextureIndex[1] = Texture_SetLoadFile("./img/Ebullet-B.png", EBULLET_SIZE, EBULLET_SIZE);
	g_EBulletTextureIndex[2] = Texture_SetLoadFile("./img/Ebullet-D.png", EBULLET_SIZE, EBULLET_SIZE);
	g_EBulletTextureIndex[3] = Texture_SetLoadFile("./img/Ebullet-C.png", EBULLET_SIZE * 5, EBULLET_SIZE * 5);

	//弾を無効にしておく
	for (int i = 0; i < EBULLET_MAX; i++)
	{
		EBulletGravity[i] = false;
		EBullet_flag[i] = false;
		g_EBulletCollision[i].cx = 0.0f;
		g_EBulletCollision[i].cy = 0.0f;
		g_EBulletCollision[i].radius = EBULLET_SIZE / 2;
		EBulletLife[i] = 0;
	}
}
void EBullet_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_EBulletTextureIndex[0], 4);
}
void EBullet_Update(void)
{
	for (int i = 0; i < EBULLET_MAX; i++)
	{
		//弾が無効だったら何もしない

		//弾が有効だったら
		//座標を更新する
		if (EBullet_flag[i])
		{
			if (EBulletGravity[i])
			{
				g_Speed[i].y += 0.05f;
			}
			g_Position[i].x += g_Speed[i].x;
			g_Position[i].y += g_Speed[i].y;
			g_EBulletCollision[i].cx = g_Position[i].x;
			g_EBulletCollision[i].cy = g_Position[i].y;
			EBulletLife[i]--;
			if (EBulletType[i] == 3)
			{
				EBullet_Create(g_Position[i].x, g_Position[i].y, 4, 0);
			}
		}

		//弾が画面外だったら
		//無効にする
		if (g_Position[i].y > SCREEN_HEIGHT || g_Position[i].y < -SCREEN_HEIGHT / 2 || g_Position[i].x < 0 || g_Position[i].x > GAME_WIDTH || EBulletLife[i] == 0)
		{
			EBullet_flag[i] = false;
			angle[i] = 0.0f;
		}
	}
}
void EBullet_Draw(void)
{
	for (int i = 0; i < EBULLET_MAX; i++)
	{
		if (EBullet_flag[i])
		{
			if (EBulletType[i] == 3)
			{
				Effect_Create(g_Position[i].x, g_Position[i].y, 60, 100, 0x88FF00FF);
				Effect_Create(g_Position[i].x, g_Position[i].y, 60, 80, 0x88FFFFFF);
			}
			else
			{
				if (g_Speed[i].x != 0 || g_Speed[i].y != 0)
				{
					Effect_Create(g_Position[i].x, g_Position[i].y, 30, 30, 0x88FF0000);
					Effect_Create(g_Position[i].x, g_Position[i].y, 30, 15, 0x88FFFFFF);
				}
				else
					Effect_Create(g_Position[i].x, g_Position[i].y, 30, 10, 0x88FFFFFF);
			}
			Sprite_DrawRotate(g_EBulletTextureIndex[EBulletType[i]], g_Position[i].x, g_Position[i].y, angle[i]);
		}
	}
}

void EBullet_Create(float x, float y, short type, short position)
{
	int number = 0;
	if (position == 1)
	{
		number = 4;
	}
	if (type == 0)
	{
		for (int j = 0; j <= number; j++)
		{
			for (int i = 0; i < EBULLET_MAX; i++)
			{
				//弾が無効だったら
				//弾を有効にして
				//弾の座標をx,yにする
				if (EBullet_flag[i] == false)
				{
					EBulletGravity[i] = false;
					EBullet_flag[i] = true;
					g_Position[i] = D3DXVECTOR2(x, y);
					g_Speed[i] = Player_GetPosition() - D3DXVECTOR2(x, y);
					D3DXVec2Normalize(&g_Speed[i], &g_Speed[i]);
					g_Speed[i] *= EBULLET_SPEED;
					EBulletType[i] = type;
					if (j != 0)
					{
						g_Speed[i] *= 0.8;
						EBulletType[i]++;
					}
					angle[i] = 0.0f;
					g_EBulletCollision[i].radius = EBULLET_SIZE / 2;
					break;
				}
			}
			if (j == 0)
			{
				x += 50;
			}
			if (j == 1)
			{
				x -= 100;
			}
			if (j == 2)
			{
				x += 50;
				y += 50;
			}
			if (j == 3)
			{
				y -= 100;
			}
		}
	}

	if (type == 1)
	{
		for (int i = 0; i < EBULLET_MAX; i++)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (EBullet_flag[i] == false)
			{
				EBulletGravity[i] = false;
				EBullet_flag[i] = true;
				g_Position[i] = D3DXVECTOR2(x, y);
				g_Speed[i].x = 0;
				g_Speed[i].y = EBULLET_SPEED;
				EBulletType[i] = 0;
				angle[i] = 0.0f;
				g_EBulletCollision[i].radius = EBULLET_SIZE / 2;
				break;
			}
		}
		
	}
	if (type == 2)
	{
		for (int i = 0; i < EBULLET_MAX; i++)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (EBullet_flag[i] == false)
			{
				EBulletGravity[i] = false;
				angle[i] = position;
				EBullet_flag[i] = true;
				g_Position[i] = D3DXVECTOR2(x, y);
				g_Speed[i].x = cos(angle[i] * (PI / 180));
				g_Speed[i].y = sin(angle[i] * (PI / 180));
				g_Speed[i] *= EBULLET_SPEED;
				EBulletType[i] = 2;
				
				g_EBulletCollision[i].radius = EBULLET_SIZE / 2;
				break;
			}
		}

	}
	if (type == 3)
	{
		for (int i = EBULLET_MAX - 1; i >= 0; i--)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (EBullet_flag[i] == false)
			{
				EBulletGravity[i] = false;
				EBullet_flag[i] = true;
				g_Position[i] = D3DXVECTOR2(x, y);
				g_Speed[i] = Player_GetPosition() - D3DXVECTOR2(x, y);
				D3DXVec2Normalize(&g_Speed[i], &g_Speed[i]);
				g_Speed[i] *= EBULLET_SPEED;
				EBulletType[i] = type;
				angle[i] = 0.0f;
				g_EBulletCollision[i].radius = EBULLET_SIZE * 5 / 2;
				break;
			}
		}
	}
	if (type == 4)
	{
		for (int i = 0; i < EBULLET_MAX; i++)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (EBullet_flag[i] == false)
			{
				EBulletGravity[i] = false;
				EBullet_flag[i] = true;
				g_Position[i] = D3DXVECTOR2(x + (rand() % 61 -30), y + (rand() % 61 - 40));
				g_Speed[i] *= 0;
				EBulletType[i] = rand() % 3;
				angle[i] = 0.0f;
				g_EBulletCollision[i].radius = EBULLET_SIZE / 2;
				EBulletLife[i] = 120;
				break;
			}
		}
	}
	if (type == 5)
	{
		for (int i = 0; i < EBULLET_MAX; i++)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (EBullet_flag[i] == false)
			{
				EBulletGravity[i] = true;
				angle[i] = position;
				EBullet_flag[i] = true;
				g_Position[i] = D3DXVECTOR2(x, y);
				g_Speed[i].x = cos(rand() % 180 * (PI / 180));
				g_Speed[i].y = -sin(rand() % 180 * (PI / 180));
				g_Speed[i] *= EBULLET_SPEED;
				EBulletType[i] = 2;

				g_EBulletCollision[i].radius = EBULLET_SIZE / 2;
				break;
			}
		}
	}
}

void EBullet_Destroy(int i)
{
	EBulletGravity[i] = false;
	EBullet_flag[i] = false;
}
bool EBullet_IsEnable(int i)
{
	return EBullet_flag[i];
}
const Circle* EBullet_GetCollision(int i)
{
	return &g_EBulletCollision[i];
}

int EBullet_MaxGet(void)
{
	return EBULLET_MAX;
}