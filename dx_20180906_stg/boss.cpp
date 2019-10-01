/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "boss.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "enemybullet.h"
#include "explosion.h"
#include "game.h"
#include "judgement.h"
#include "item.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define BOSS_HEIGHT	(146)
#define BOSS_WIDTH	(146)
#define BOSS_MAXLIFE (10000)
#define PI			(3.141592)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_TextureIndex[3] = { TEXTURE_INVALID_INDEX };
static D3DXVECTOR2 g_Position;
static Circle g_BossCollision;
static D3DXVECTOR2 g_Speed;
static float angle;
static int BossLife;
//static float g_Speed.x;
//static float g_Speed.y;
static int BossMotion;
static int textureanime;
static bool flag;
static int frame;
static bool clear;
static int shotAngle;
static int gage;
static int wait;



void Boss_Initialize(void)
{
	//テクスチャの読込予約
	g_TextureIndex[0] = Texture_SetLoadFile("./img/boss.png", BOSS_WIDTH * 2, BOSS_HEIGHT * 3);
	g_TextureIndex[1] = Texture_SetLoadFile("./img/lifegage.png",480, 16);
	g_TextureIndex[2] = Texture_SetLoadFile("./img/lifebar.png", 1, 16);

	g_Position.x = GAME_WIDTH / 2;
	g_Position.y = -BOSS_HEIGHT;
	g_BossCollision.cx = g_Position.x;
	g_BossCollision.cy = g_Position.y;
	g_BossCollision.radius = BOSS_WIDTH / 2;
	BossLife = BOSS_MAXLIFE;
	BossMotion = 0;
	g_Speed.x = 0;
	g_Speed.y = 5;
	angle = 0;
	shotAngle = 0;
	textureanime = 0;
	flag = true;
	frame = 0;
	clear = false;
	gage = 0;
	wait = 0;
}
void Boss_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_TextureIndex[0], 3);
}
void Boss_Update(void)
{
	if (BossLife == 2000)
	{
		Explosion_Create(g_Position);
	}
	if (BossLife <= 0)
	{
		flag = true;
		g_Speed.x = -1;

		if (frame % 60 == 0)
		{
			Explosion_Create(D3DXVECTOR2(g_Position.x + (rand() % 101 - 50), g_Position.y + (rand() % 101 - 50)));
		}
		if (g_Position.x < 0 - (BOSS_WIDTH / 2) || g_Position.x > GAME_WIDTH + (BOSS_WIDTH / 2))
			if(clear==false)
				Boss_Destroy();
	}
	g_Position.x += g_Speed.x;
	g_Position.y += g_Speed.y;
	g_BossCollision.cx = g_Position.x;
	g_BossCollision.cy = g_Position.y;
	textureanime++;
	if (flag && BossLife > 0)
	{

		g_Speed.y *= 0.99;
		if (g_Position.y >= BOSS_HEIGHT / 2)
		{
			g_Speed.y = 0;
			wait++;
			if (wait > 30)
			{
				flag = false;
				PlaySound(SOUND_LABEL_BGM000);
			}
		}
	}
	if (flag == false && BossLife > 0)
	{
		if (gage < 30)
			gage++;
		angle++;
		g_Speed.x = cos(angle * (PI / 180)) * 3;
		g_Speed.y = sin(angle * (PI / 180));
	}
	if (BossLife > 8000)
	{
		if (frame % 120 == 0)
			if (flag == false)
				EBullet_Create(g_Position.x, g_Position.y, 3, 0);
	}
	else if (BossLife > 6000)
	{
		if (frame % 120 == 0)
			EBullet_Create(g_Position.x, g_Position.y, 3, 0);
		if (frame % 120 == 60)
			EBullet_Create(g_Position.x + 70, g_Position.y, 0, 1);
		if (frame % 120 == 90)
			EBullet_Create(g_Position.x - 70, g_Position.y, 0, 1);
	}
	else if (BossLife > 4000)
	{
		shotAngle++;
		if (frame % 120 == 0)
			EBullet_Create(g_Position.x, g_Position.y, 3, 0);
		if (frame % 20 == 0)
			EBullet_Create(g_Position.x + 70, g_Position.y, 2, shotAngle);
		if (frame % 20 == 10)
			EBullet_Create(g_Position.x - 70, g_Position.y, 2, shotAngle + 180);
	}
	else if (BossLife > 2000)
	{
		shotAngle++;
		if (frame % 120 == 0)
			EBullet_Create(g_Position.x, g_Position.y, 3, 0);
		if (frame % 60 == 0 || frame % 60 == 30)
			for(int i=0;i<15;i++)
				EBullet_Create(g_Position.x, g_Position.y, 5, 10);
	}
	else if (BossLife > 0)
	{
		shotAngle++;
		if (frame % 120 == 0)
			EBullet_Create(g_Position.x, g_Position.y, 3, 0);
		if (frame % 10 == 0)
			EBullet_Create(g_Position.x + 70, g_Position.y, 2, shotAngle);
		if (frame % 10 == 5)
			EBullet_Create(g_Position.x - 70, g_Position.y, 2, shotAngle + 180);
		if (frame % 60 == 0 || frame % 60 == 30)
			for (int i = 0; i<15; i++)
				EBullet_Create(g_Position.x, g_Position.y, 5, 10);
	}
	
	frame++;
}
void Boss_Draw(void)
{
	if(BossLife < 2000)
		if(BossLife > 0)
			Sprite_Draw(g_TextureIndex[0], g_Position.x - (BOSS_WIDTH / 2), g_Position.y - (BOSS_HEIGHT / 2), textureanime * BOSS_WIDTH, BOSS_HEIGHT, BOSS_WIDTH, BOSS_HEIGHT);
		else
			Sprite_Draw(g_TextureIndex[0], g_Position.x - (BOSS_WIDTH / 2), g_Position.y - (BOSS_HEIGHT / 2), textureanime * BOSS_WIDTH, BOSS_HEIGHT * 2, BOSS_WIDTH, BOSS_HEIGHT);
	else
		Sprite_Draw(g_TextureIndex[0], g_Position.x - (BOSS_WIDTH / 2), g_Position.y - (BOSS_HEIGHT / 2), textureanime * BOSS_WIDTH, 0, BOSS_WIDTH, BOSS_HEIGHT);
	Sprite_Draw(g_TextureIndex[2], 42, -25 + gage, 0, 0, 420 * ((float)BossLife / BOSS_MAXLIFE), 16);
	Sprite_Draw(g_TextureIndex[1], 10, -25 + gage, 0, 0, 480, 16);
}

int Boss_Damage(void)
{
	BossLife--;
	if (BossLife % 1000 == 0)
	{
		for(int i=0;i<20;i++)
			Item_Create(g_Position.x + (rand() % 201 -100), g_Position.y + (rand() % 201 - 100), 1);
		Explosion_Create(g_Position);
	}
	return 1;
}
void Boss_Destroy(void)
{
	Boss_Score();
	Game_Clear();
	clear = true;
}

const Circle* Boss_GetCollision(void)
{
	return &g_BossCollision;
}

bool Boss_IsEnable()
{
	if (flag)
		return false;
	else
		return true;
}