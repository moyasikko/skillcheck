/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "judgement.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "item.h"
#include "player.h"
#include "enemybullet.h"
#include "boss.h" 
#include "sound.h"

static int g_Score;
static int g_GetScore;

void Judgement_Initialize(void)
{
	g_Score = 0;
	g_GetScore = 0;
}

void Judgement_Update(void)
{
	//プレイヤーの弾は有効か？
	//敵は有効か？
	for (int i = 0; i < Bullet_MaxGet(); i++)
	{
		for (int j = 0; j < Enemy_MaxGet(); j++)
		{
			if (Bullet_IsEnable(i) && Enemy_IsEnable(j))
			{
				//当たり判定
				if (Collision_IsHitCircle(Bullet_GetCollision(i), Enemy_GetCollision(j)))
				{
					//当たっていたらプレイヤーの弾と敵を無効にする
					g_Score += Enemy_Damage(j);
					Bullet_Destroy(i, j);
				}
			}
		}
		if (Bullet_IsEnable(i) && Boss_IsEnable())
		{
			//当たり判定
			if (Collision_IsHitCircle(Bullet_GetCollision(i), Boss_GetCollision()))
			{
				//当たっていたらプレイヤーの弾と敵を無効にする
				g_Score += Boss_Damage();
				Bullet_Destroy(i, -1);
			}
		}
	}
	//アイテムは有効か？
	for (int i = 0; i < Item_MaxGet(); i++)
	{
		if (Item_IsEnable(i))
		{
			//当たり判定
			if (Collision_IsHitCircle(Item_GetCollision(i), Player_GetCollision()))
			{
				PlaySound(SOUND_LABEL_SE_GETITEM);
				if (Item_TypeGet(i) == 0)
				{
					//当たっていたらプレイヤーのパワーを増やす
					g_Score += Player_Powerup();
				}
				else if (Item_TypeGet(i) == 1)
				{
					g_Score += 1000;
					g_GetScore++;
					/*if (g_GetScore % 50 == 0)
					{
						Player_LifeUp();
					}*/
				}
				else if(Item_TypeGet(i) == 2)
				{
					Player_LifeUp();
				}
				Item_Destroy(i);
			}
		}
	}
	

	//敵は有効か？
	for (int i = 0; i < Enemy_MaxGet(); i++)
	{
		if (Enemy_IsEnable(i))
		{
			if (Muteki() == 0)
			{
				//当たり判定
				if (Collision_IsHitCircle(Enemy_GetCollision(i), Player_GetCollision()))
				{
					//当たっていたらプレイヤーのライフを減らす
					Player_Damage();
				}
			}
		}
	}
	if (Muteki() == 0)
	{
		//当たり判定
		if (Collision_IsHitCircle(Boss_GetCollision(), Player_GetCollision()))
		{
			//当たっていたらプレイヤーのライフを減らす
			Player_Damage();
		}
	}
	//敵弾は有効か？
	for (int i = 0; i < EBullet_MaxGet(); i++)
	{
		if (EBullet_IsEnable(i))
		{
			if (Muteki() == 0)
			{
				//当たり判定
				if (Collision_IsHitCircle(EBullet_GetCollision(i), Player_GetCollision()))
				{
					//当たっていたらプレイヤーのライフを減らす
					Player_Damage();
					EBullet_Destroy(i);
				}
			}
		}
	}

	if (g_GetScore >= 50)
	{
		Player_LifeUp();
		g_GetScore -= 50;
	}

}
int Score(void)
{
	return g_Score;
}
void Boss_Score()
{
   	g_Score += 10000000;
}