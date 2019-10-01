/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "bullet.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "enemy.h"
#include "item.h"
#include "debug_primitive.h"
#include "player.h"
#include <d3dx9.h>
#include "input.h"
#include "sound.h"
#include "explosion.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define BULLET_MAX		(256)
#define BULLET_SIZE		(20)
#define BULLET_SPEED	(15)
#define BULLET_TYPE		(2)
#define TEXTURE_BLOCK	(192)
#define PI			(3.141592)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_BulletTextureIndex[BULLET_TYPE] = { TEXTURE_INVALID_INDEX };
static int g_ExplosionTextureIndex = TEXTURE_INVALID_INDEX;
static bool Bullet_flag[BULLET_MAX];
static D3DXVECTOR2 g_Position[BULLET_MAX];
static D3DXVECTOR2 g_Speed[BULLET_MAX];
static float angle[BULLET_MAX];
static Circle g_BulletCollision[BULLET_MAX];
static bool Bullet_Explosion[BULLET_MAX];
static int tx[BULLET_MAX];
static int ty[BULLET_MAX];
static short BulletType[BULLET_MAX];


void Bullet_Initialize(void)
{
	//テクスチャの読込予約
	g_BulletTextureIndex[0] = Texture_SetLoadFile("./img/bullet-A.png", BULLET_SIZE, BULLET_SIZE);
	g_BulletTextureIndex[1] = Texture_SetLoadFile("./img/bullet-B.png", 30, 30);
	g_ExplosionTextureIndex = Texture_SetLoadFile("./img/explosion.png", TEXTURE_BLOCK * 5, TEXTURE_BLOCK * 5);

	//弾を無効にしておく
	for (int i = 0; i < BULLET_MAX; i++)
	{
		Bullet_flag[i] = false;
		Bullet_Explosion[i] = false;
		g_BulletCollision[i].cx = 0.0f;
		g_BulletCollision[i].cy = 0.0f;
		g_BulletCollision[i].radius = BULLET_SIZE / 2;
	}
	
}
void Bullet_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_BulletTextureIndex[0], 2);
	Texture_Release(&g_ExplosionTextureIndex, 1);
}
void Bullet_Update(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が無効だったら何もしない

		//弾が有効だったら
		//座標を更新する
		if (Bullet_flag[i])
		{
			if (Bullet_Explosion[i] == false)
			{
				g_Position[i].x += g_Speed[i].x;
				g_Position[i].y += g_Speed[i].y;
				//angle[i] += 9.0f;
				g_BulletCollision[i].cx = g_Position[i].x;
				g_BulletCollision[i].cy = g_Position[i].y;
			}
			else
			{
				tx[i]++;
				if (tx[i] == 10)
				{
					tx[i] = 0;
					ty[i]++;
					if (ty[i] == 4)
					{
						Bullet_flag[i] = false;
					}
				}
			}
		}

		//弾が画面外だったら
		//無効にする
		if (g_Position[i].y < 0)
		{
			Bullet_flag[i] = false;
			angle[i] = 0.0f;
		}
	}
}
void Bullet_Draw(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//弾が有効だったら
		//弾の絵を描く
		if (Bullet_flag[i])
		{
			if (Bullet_Explosion[i] == false)
			{
				Sprite_DrawRotate(g_BulletTextureIndex[BulletType[i]], g_Position[i].x, g_Position[i].y, angle[i]);
				DebugPrimitive_BatchDraw(g_BulletCollision[i].cx, g_BulletCollision[i].cy, g_BulletCollision[i].radius, D3DCOLOR_RGBA(255, 255, 0, 255));
			}
			else
			{
				Sprite_Draw(g_ExplosionTextureIndex, g_Position[i].x - (TEXTURE_BLOCK / 2), g_Position[i].y - (TEXTURE_BLOCK / 2), (tx[i] / 2) * TEXTURE_BLOCK, ty[i] * TEXTURE_BLOCK, TEXTURE_BLOCK, TEXTURE_BLOCK);
			}
		}
	}
}

void Bullet_Create(float x, float y, short type, short position)
{
	if (type == 0)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (Bullet_flag[i] == false)
			{
				Bullet_flag[i] = true;
				g_Position[i] = D3DXVECTOR2(x - 15, y);
				g_Speed[i] = D3DXVECTOR2(0, -BULLET_SPEED);
				Bullet_Explosion[i] = false;
				tx[i] = 0.0f;
				ty[i] = 0.0f;
				angle[i] = 0.0f;
				BulletType[i] = type;
				break;
			}
		}
		for (int i = 0; i < BULLET_MAX; i++)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (Bullet_flag[i] == false)
			{
				Bullet_flag[i] = true;
				g_Position[i] = D3DXVECTOR2(x + 15, y);
				g_Speed[i] = D3DXVECTOR2(0, -BULLET_SPEED);
				Bullet_Explosion[i] = false;
				tx[i] = 0.0f;
				ty[i] = 0.0f;
				angle[i] = 0.0f;
				BulletType[i] = type;
				break;
			}
		}
	}
	if (type == 1)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (Bullet_flag[i] == false)
			{
				Bullet_flag[i] = true;
				if (Keyboard_IsPress(DIK_M) || Keyboard_IsPress(DIK_LSHIFT) || GamePad_IsPress(XINPUT_GAMEPAD_LEFT_SHOULDER))
					angle[i] = 0.0f;
				else
				{
					angle[i] = 0.0f;
					if (position == 1)//右
						angle[i] = 10.0f;
					if(position == 2)//左
						angle[i] = -10.0f;
					if (position == 3)//右
						angle[i] = 20.0f;
					if (position == 4)//右
						angle[i] = -20.0f;
					
				}
				g_Position[i] = D3DXVECTOR2(x, y);
				g_Speed[i] = D3DXVECTOR2(cos((90 - angle[i]) * (PI / 180))*BULLET_SPEED, -BULLET_SPEED * sin((90-angle[i]) * (PI / 180)));
				Bullet_Explosion[i] = false;
				tx[i] = 0.0f;
				ty[i] = 0.0f;
				BulletType[i] = type;
				break;
			}
		}
		/*for (int i = 0; i < BULLET_MAX; i++)
		{
			//弾が無効だったら
			//弾を有効にして
			//弾の座標をx,yにする
			if (Bullet_flag[i] == false)
			{
				Bullet_flag[i] = true;
				if (Keyboard_IsPress(DIK_M))
					angle[i] = 0.0f;
				else
				{
					angle[i] = -10.0f;
					if(position == 1)//左
						angle[i] = 5.0f;
				}
				g_Position[i] = D3DXVECTOR2(x, y);
				g_Speed[i] = D3DXVECTOR2(-cos((90 + angle[i]) * (PI / 180)) * BULLET_SPEED, -BULLET_SPEED * sin((90 + angle[i]) * (PI / 180)));
				Bullet_Explosion[i] = false;
				tx[i] = 0.0f;
				ty[i] = 0.0f;
				BulletType[i] = type;
				break;
			}
		}*/
	}
}

void Bullet_Destroy(int i, int j)
{
	if (Enemy_LifeGet(j) == 0 && j != -1)
	{
		//Bullet_Explosion[i] = true;
		Explosion_Create(g_Position[i]);
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
	}
	//else
		Bullet_flag[i] = false;
	tx[i] = 0;
	ty[i] = 0;
}
bool Bullet_IsEnable(int i)
{
	if(Bullet_flag[i])
		if(Bullet_Explosion[i] == false)
			return true;
	return false;
}
const Circle* Bullet_GetCollision(int i)
{
	return &g_BulletCollision[i];
}
int Bullet_MaxGet(void)
{
	return BULLET_MAX;
}
