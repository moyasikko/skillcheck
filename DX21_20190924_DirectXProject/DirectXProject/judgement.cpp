/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "collision.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "enemybullet.h"
#include "item.h"
#include "sound.h"
#include "bomb.h"

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
static void hitCheck_Bullet_Enemy(void);
static void hitCheck_Player_Enemy(void);
static void hitCheck_Player_EBullet(void);
static void hitCheck_Player_Item(void);
static void hitCheck_Bomb_EBullet(void);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_Score;

void Judgement_Update(void)
{
	hitCheck_Bullet_Enemy();
	hitCheck_Player_Enemy();
	hitCheck_Player_EBullet();
	hitCheck_Player_Item();
	hitCheck_Bomb_EBullet();
}

static void hitCheck_Bullet_Enemy(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!Bullet_Flag(i))
			continue;
		for (int j = 0; j < ENEMY_MAX; j++)
		{
			if (!Enemy_Flag(j))
				continue;
			if (Circle_Circle(Bullet_GetCollision(i), Enemy_GetCollision(j)))
			{
				Bullet_Destroy(i);
				Enemy_Damage(j);
			}
	
		}
	}
}

static void hitCheck_Player_Enemy(void)
{
	if (!Player_GetFlag())
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (Enemy_Flag(i))
			{
				if (Circle_Circle(Player_GetCollision(), Enemy_GetCollision(i)))
				{
					Player_Damege();
				}
			}
		}
	}
}

static void hitCheck_Player_EBullet(void)
{
	if (!Player_GetFlag())
	{
		for (int i = 0; i < ENEMYBULLET_MAX; i++)
		{
			if (EnemyBullet_Flag(i))
			{
				if (Circle_Circle(Player_GetCollision(), EnemyBullet_GetCollision(i)))
				{
					Player_Damege();
					EnemyBullet_Destroy(i);
					break;
				}
			}
		}
	}
}

static void hitCheck_Player_Item(void)
{

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (Item_Flag(i))
		{
			if (Circle_Circle(Player_GetCollision(), Item_GetCollision(i)))
			{
				PlaySound(SOUND_LABEL_SE_GETITEM);
				Player_PowerUp();
				Item_Destroy(i);
			}
		}
	}

}

static void hitCheck_Bomb_EBullet(void)
{
	if (Bomb_Flag())
	{
		for (int i = 0; i < ENEMYBULLET_MAX; i++)
		{
			if (EnemyBullet_Flag(i))
			{
				if (Circle_Circle(Bomb_GetCollision(), EnemyBullet_GetCollision(i)))
				{
					EnemyBullet_Destroy(i);
					g_Score += 10;
					break;
				}
			}
		}
	}
}

int GetScore(void)
{
	return g_Score;
}
void AddScore(int score)
{
	g_Score += score;
}