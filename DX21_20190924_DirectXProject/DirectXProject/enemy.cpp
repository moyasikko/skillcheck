/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include "enemy.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "enemybullet.h"
#include "debug.h"
#include "score.h"
#include "debug_font.h"
#include "item.h"
#include "explosion.h"
#include "judgement.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define ENEMY_SPEED (2.0f)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct ENEMY
{
	ENEMY_TYPE Type;
	ENEMY_MOVETYPE Movetype;
	ENEMY_SHOTTYPE Shottype;
	D3DXVECTOR2 Position;
	D3DXVECTOR2 Speed;
	int Life;
	int Angle;
	bool Flag;
	bool Flag2;
	Circle Collision;
	int Age;
	int Muki;
	bool Hidanflag;
	int Hidancount;
	int Alpha;
	ITEM_TYPE Itemtype;
};

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static ENEMY g_Enemy[ENEMY_MAX];

static int g_Texture[TYPE_MAX];


void Enemy_Init(void)
{
	g_Texture[0] = Texture_SetLoadFile("./sozai/img/enemyA.png", 124, 248);
	g_Texture[1] = Texture_SetLoadFile("./sozai/img/enemyB.png", 124, 248);
	g_Texture[2] = Texture_SetLoadFile("./sozai/img/enemyC.png", 124, 248);
	g_Texture[3] = Texture_SetLoadFile("./sozai/img/enemyD.png", 124, 248);
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].Flag = false;
		g_Enemy[i].Flag2 = false;
		g_Enemy[i].Speed.y = ENEMY_SPEED;
		g_Enemy[i].Speed.x = 0.0f;
		g_Enemy[i].Life = 0;
		g_Enemy[i].Collision.radius = 50 / 2;
		g_Enemy[i].Age = 0;
		g_Enemy[i].Muki = 0;
		g_Enemy[i].Hidanflag = false;
		g_Enemy[i].Hidancount = 0;
		g_Enemy[i].Alpha = 255;
	}
}

void Enemy_Uninit(void)
{
	Texture_Destroy(g_Texture, 1);
}

void Enemy_Update(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (!g_Enemy[i].Flag2)
		{
			continue;
		}

		//敵の座標の更新
		switch (g_Enemy[i].Movetype)
		{
		case MOVETYPE5:
			g_Enemy[i].Speed.x = cos(g_Enemy[i].Angle * 3.14 / 180) * ENEMY_SPEED;
			break;
		case MOVETYPE6:
			if (g_Enemy[i].Age < 1500)
			{
				g_Enemy[i].Speed.y *= 0.99;
			}
			else
			{
				g_Enemy[i].Speed.y -= 0.02;
			}
			break;
		default:
			break;
		}
		if (g_Enemy[i].Speed.x > 1.0f)
		{
			g_Enemy[i].Muki = 2;
		}
		else if (g_Enemy[i].Speed.x < -1.0f)
		{
			g_Enemy[i].Muki = 1;
		}
		else
		{
			g_Enemy[i].Muki = 0;
		}
		g_Enemy[i].Angle += 1;
		if (!g_Enemy[i].Flag)
		{
			if (g_Enemy[i].Speed.x > 0)
				g_Enemy[i].Angle += 5;
			else
				g_Enemy[i].Angle -= 5;
		}
		g_Enemy[i].Position += g_Enemy[i].Speed;
		g_Enemy[i].Collision.position = g_Enemy[i].Position;

		switch (g_Enemy[i].Shottype)
		{
		case SHOTTYPE1:
			if (g_Enemy[i].Age % 180 == 0 && g_Enemy[i].Flag)
			{
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE1, 0);
			}
			break;
		case SHOTTYPE2:
			if(g_Enemy[i].Age % 40 == 0 && g_Enemy[i].Flag)
			{
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90);
			}
			break;
		case SHOTTYPE3:
			if (g_Enemy[i].Age % 5 == 0 && g_Enemy[i].Flag)
			{
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, g_Enemy[i].Age);
			}
			break;
		case SHOTTYPE4:
			if (g_Enemy[i].Age % 5 == 0 && g_Enemy[i].Flag)
			{
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, -g_Enemy[i].Age + 180);
			}
			break;
		case SHOTTYPE5:
			if (g_Enemy[i].Age % 40 == 0 && g_Enemy[i].Flag)
			{
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90);
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 60);
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 120);
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 180);
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 240);
				EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 300);
			}
			break;

		default:
			break;
		}
		
		if (g_Enemy[i].Position.x < 0 - ENEMY_SIZE ||
			g_Enemy[i].Position.x > GAME_WIDTH + ENEMY_SIZE ||
			g_Enemy[i].Position.y > SCREEN_HEIGHT + ENEMY_SIZE ||
			g_Enemy[i].Position.y < 0 - ENEMY_SIZE)
		{
			g_Enemy[i].Flag = false;
			g_Enemy[i].Flag2 = false;
		}
		if (g_Enemy[i].Hidanflag)
		{
			g_Enemy[i].Hidancount--;
			if (g_Enemy[i].Hidancount % 2 == 0)
			{
				g_Enemy[i].Alpha = 255;
			}
			else
			{
				g_Enemy[i].Alpha = 64;
			}
			if (g_Enemy[i].Hidancount <= 0)
			{
				g_Enemy[i].Hidanflag = false;
			}
		}
		g_Enemy[i].Age++;
	}
}

void Enemy_Draw(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
		if (g_Enemy[i].Flag2)
		{
			if (g_Enemy[i].Flag)
			{
				Sprite_DrawCut(g_Texture[g_Enemy[i].Type], g_Enemy[i].Position.x, g_Enemy[i].Position.y, (g_Enemy[i].Age / 2) % 2 * 62, g_Enemy[i].Muki * 62, 62, 62, D3DCOLOR_ARGB(g_Enemy[i].Alpha, 255, 255, 255));
				Debug_SetColor(D3DCOLOR(0xFF00FF00));
				Debug_Draw(g_Enemy[i].Collision.position.x, g_Enemy[i].Collision.position.y, g_Enemy[i].Collision.radius);
			}
			else
			{
				Sprite_DrawCR(g_Texture[g_Enemy[i].Type], g_Enemy[i].Position.x, g_Enemy[i].Position.y, 0, 186, 62, 62, g_Enemy[i].Angle);
			}
		}
	//DebugFont_Draw(0, 80, "ENEMY_POSITION_X:%06.2f\nENEMY_POSITION_Y:%06.2f", g_Enemy[0].Position.x, g_Enemy[0].Position.y);
	//DebugFont_Draw(0, 120, "ENEMY_SPEED_X:%06.2f\nENEMY_SPEED_Y:%06.2f", g_Enemy[0].Speed.x, g_Enemy[0].Speed.y);
	DebugFont_Draw(20, 160, "ENEMY_muki:%d", g_Enemy[0].Muki);
}

void Enemy_Create(float x, float y, int life, ENEMY_TYPE type, ENEMY_MOVETYPE move, ENEMY_SHOTTYPE shot, ITEM_TYPE item)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].Flag || g_Enemy[i].Flag2)
		{
			continue;
		}
		g_Enemy[i].Life = life;
		g_Enemy[i].Type = type;
		g_Enemy[i].Movetype = move;
		g_Enemy[i].Shottype = shot;
		g_Enemy[i].Itemtype = item;
		switch (g_Enemy[i].Movetype)
		{
		case MOVETYPE1://下
			g_Enemy[i].Speed.x = 0.0f;
			g_Enemy[i].Speed.y = ENEMY_SPEED;
			break;
		case MOVETYPE2://上
			g_Enemy[i].Speed.x = 0.0f;
			g_Enemy[i].Speed.y = -ENEMY_SPEED;
			break;
		case MOVETYPE3://右
			g_Enemy[i].Speed.x = ENEMY_SPEED;
			g_Enemy[i].Speed.y = 0.0f;
			break;
		case MOVETYPE4://左
			g_Enemy[i].Speed.x = -ENEMY_SPEED;
			g_Enemy[i].Speed.y = 0.0f;
			break;
		case MOVETYPE5://フラフラ
			g_Enemy[i].Speed.x = 0.0f;
			g_Enemy[i].Speed.y = ENEMY_SPEED*0.5f;
			g_Enemy[i].Angle = rand() % 360;
			break;
		case MOVETYPE6://下
			g_Enemy[i].Speed.x = 0.0f;
			g_Enemy[i].Speed.y = ENEMY_SPEED;
			break;
		case MOVETYPE7://右下
			g_Enemy[i].Speed.x = cos((20 + (rand() % 51)) * 3.14 / 180) * ENEMY_SPEED;
			g_Enemy[i].Speed.y = sin((20 + (rand() % 51)) * 3.14 / 180) * ENEMY_SPEED;
			break;
		case MOVETYPE8://左下
			g_Enemy[i].Speed.x = cos((110 + (rand() % 51)) * 3.14 / 180) * ENEMY_SPEED;
			g_Enemy[i].Speed.y = sin((110 + (rand() % 51)) * 3.14 / 180) * ENEMY_SPEED;
			break;
		default:
			break;
		}
		
		g_Enemy[i].Position = D3DXVECTOR2(x, y);
		g_Enemy[i].Collision.position = g_Enemy[i].Position;
		g_Enemy[i].Flag = true;
		g_Enemy[i].Flag2 = true;
		g_Enemy[i].Age = 0;
		break;
	}
}

bool Enemy_Flag(int i)
{
	return g_Enemy[i].Flag;
}

void Enemy_Damage(int i)
{
	g_Enemy[i].Life--;
	g_Enemy[i].Hidanflag = true;
	g_Enemy[i].Hidancount = 60;
	if (g_Enemy[i].Life <= 0)
	{
		Enemy_Destroy(i);
	}
	AddScore(1);
}

void Enemy_Destroy(int i)
{
	g_Enemy[i].Flag = false;
	g_Enemy[i].Angle = (rand() % 91) - 45;
	g_Enemy[i].Speed.x = sin(g_Enemy[i].Angle * (3.14 / 180));
	g_Enemy[i].Speed.y = -cos(g_Enemy[i].Angle * (3.14 / 180));
	g_Enemy[i].Speed *= ENEMY_SPEED * 3;
	g_Enemy[i].Angle = 0;
	switch (g_Enemy[i].Type)
	{
	case TYPE1:
		Item_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, g_Enemy[i].Itemtype);
		break;
	case TYPE2:
		for (int j = 0; j < 10; j++)
		{
			Item_Create(g_Enemy[i].Position.x + (rand() % 51 - 25), g_Enemy[i].Position.y + (rand() % 51 - 25), g_Enemy[i].Itemtype);
		}
		break;
	case TYPE3:
		Item_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, g_Enemy[i].Itemtype);
		EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90);
		EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 60);
		EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 120);
		EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 180);
		EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 240);
		EnemyBullet_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y, BULLETTYPE2, 90 + 300);
		break;
	case TYPE4:
		for (int j = 0; j < 30; j++)
		{
			Item_Create(g_Enemy[i].Position.x + (rand() % 81 - 40), g_Enemy[i].Position.y + (rand() % 81 - 40), g_Enemy[i].Itemtype);
		}
		break;
	default:
		break;
	}
	
	Explosion_Create(g_Enemy[i].Position.x, g_Enemy[i].Position.y);
	AddScore(100);
}

const Circle* Enemy_GetCollision(int i)
{
	return  &g_Enemy[i].Collision;
}

D3DXVECTOR2 Enemy_GetPosition(int i)
{
	return g_Enemy[i].Position;
}