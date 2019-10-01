/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "enemy.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "item.h"
#include "debug_primitive.h"
#include "enemybullet.h"
#include "sound.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define ENEMY_MAX		(50)
#define ENEMY_WIDTH		(62)
#define ENEMY_HEIGHT	(62)
#define ENEMY_TYPE		(10)
#define PI			(3.141592)
#define ENEMY_SPEED		(2)



/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_TextureIndex[ENEMY_TYPE] = { TEXTURE_INVALID_INDEX };
static bool Enemy_flag[ENEMY_MAX];
static bool Enemy_dead[ENEMY_MAX];
static D3DXVECTOR2 g_Position[ENEMY_MAX];
static Circle g_EnemyCollision[ENEMY_MAX];
static D3DXVECTOR2 g_Speed[ENEMY_MAX];
static float angle[ENEMY_MAX];
static int textureanime[ENEMY_MAX];
static short enemyVector[ENEMY_MAX];
static short EnemyLife[ENEMY_MAX];
static bool EnemyDrop[ENEMY_MAX];
static short EnemyDroptype[ENEMY_MAX];
//static int EnemyMotion[ENEMY_MAX];
static int EnemyAge[ENEMY_MAX];
static int EnemyMovePattern[ENEMY_MAX];
static int EnemyShotPattern[ENEMY_MAX];
static short EnemyType[ENEMY_MAX];
static int frame;

struct ENEMY
{
	bool Flag;
	bool Dead;
	D3DXVECTOR2 Position;
	D3DXVECTOR2 Speed;
	Circle Collision;
	float Angle;
	int TextureAnime;
	short Vector;
	short Life;
	bool Drop;
	short DropType;
	//int Motion;
	int Age;
	int MovePattern;
	int ShotPattern;
	short Type;
};
static ENEMY Enemy[ENEMY_MAX];



void Enemy_Initialize(void)
{
	//テクスチャの読込予約
	g_TextureIndex[0] = Texture_SetLoadFile("./img/enemyA.png", 124, 248);
	g_TextureIndex[1] = Texture_SetLoadFile("./img/enemyB.png", 124, 248);
	g_TextureIndex[2] = Texture_SetLoadFile("./img/enemyC.png", 124, 248);
	g_TextureIndex[3] = Texture_SetLoadFile("./img/enemyAd.png", 124, 248);
	g_TextureIndex[4] = Texture_SetLoadFile("./img/enemyBd.png", 124, 248);
	g_TextureIndex[5] = Texture_SetLoadFile("./img/enemyCd.png", 124, 248);
	

	//弾を無効にしておく
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		Enemy[i].Flag = false;
		Enemy[i].Dead = false;
		Enemy[i].Collision.cx = 0.0f;
		Enemy[i].Collision.cy = 0.0f;
		Enemy[i].Collision.radius = ENEMY_WIDTH / 3;
	}
	frame = 0;
}
void Enemy_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_TextureIndex[0], ENEMY_TYPE);
}
void Enemy_Update(void)
{
	frame++;
	if (frame >= 300 && frame % 30 == 0 && frame <= 600)
	{
		Enemy_Create(0, 40, 0, 0, 0);
	}
	if (frame >= 900 && frame % 30 == 0 && frame <= 1200)
	{
		Enemy_Create(GAME_WIDTH, 80, 1, 0, 0);
	}
	if (frame >= 1500 && frame % 30 == 0 && frame <= 1800)
	{
		Enemy_Create(0, 120, 0, 0, 0);
	}
	if (frame >= 2100 && frame % 30 == 0 && frame <= 2400)
	{
		Enemy_Create(40, SCREEN_HEIGHT, 4, 0, 0);
	}
	if (frame >= 2700 && frame % 30 == 0 && frame <= 3000)
	{
		Enemy_Create(GAME_WIDTH - 40, SCREEN_HEIGHT, 5, 0, 0);
	}
	if (frame == 3500)
	{
		Enemy_Create(100, 0, 6, 1, 0);
		Enemy_Create(GAME_WIDTH - 100, 0, 6, 1, 0);
	}
	if (frame >= 3800 && frame % 30 == 0 && frame <= 4100)
	{
		Enemy_Create(0, 40, 0, 0, rand() % 2);
		Enemy_Create(GAME_WIDTH, 80, 1, 0, rand() % 2);
	}
	if (frame >= 4100 && frame % 30 == 0 && frame <= 4400)
	{
		Enemy_Create(GAME_WIDTH - 40, SCREEN_HEIGHT, 5, 0, 0);
	}
	if (frame >= 4400 && frame % 30 == 0 && frame <= 4700)
	{
		Enemy_Create(40, SCREEN_HEIGHT, 4, 0, 0);
	}
	if (frame >= 5000 && frame % 30 == 0 && frame <= 6000)
	{
		Enemy_Create(GAME_WIDTH / 2, 0, 7, 2, 0);
	}
	if (frame == 5400)
	{
		Enemy_Create(100, 0, 6, 1, 0);
		Enemy_Create(GAME_WIDTH - 100, 0, 6, 1, 0);
	}
	if (frame == 6000)
	{
		Enemy_Create(GAME_WIDTH / 2, 0, 8, 3, 0);
	}
	if (frame >= 6300 && frame % 30 == 0 && frame <= 6600)
	{
		Enemy_Create(0, 40, 9, 4, 0);
	}
	if (frame >= 6900 && frame % 30 == 0 && frame <= 7200)
	{
		Enemy_Create(GAME_WIDTH, 80, 10, 4, 0);
	}
	if (frame == 6750)
	{
		Enemy_Create(100, 0, 6, 1, 0);
		Enemy_Create(GAME_WIDTH - 100, 0, 6, 1, 0);
	}
	if (frame >= 7500 && frame % 30 == 0 && frame <= 7800)
	{
		Enemy_Create(GAME_WIDTH - 40, SCREEN_HEIGHT, 5, 4, 0);
	}
	if (frame >= 8100 && frame % 30 == 0 && frame <= 8400)
	{
		Enemy_Create(40, SCREEN_HEIGHT, 4, 4, 0);
	}
	if (frame == 8000)
	{
		Enemy_Create(GAME_WIDTH / 4, 0, 8, 3, 0);
		Enemy_Create(GAME_WIDTH / 4 * 3, 0, 8, 3, 0);
	}
	
	
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//弾が無効だったら何もしない

		//弾が有効だったら
		//座標を更新する
		if (Enemy[i].Flag)
		{
			if (Enemy[i].Dead == false)
			{
				if (Enemy[i].MovePattern == 0)
				{

				}
				if (Enemy[i].MovePattern == 4)
				{
					if (Enemy[i].Position.y <= 300 && Enemy[i].Angle < 90)
					{
						Enemy[i].Angle += 0.5f;
						Enemy[i].Speed.x = sin(Enemy[i].Angle * (PI / 180)) * ENEMY_SPEED;
						Enemy[i].Speed.y = -cos(Enemy[i].Angle * (PI / 180)) * ENEMY_SPEED;
					}
				}
				if (Enemy[i].MovePattern == 5)
				{
					if (Enemy[i].Position.y <= 300 && Enemy[i].Angle > -90)
					{
						Enemy[i].Angle -= 0.5f;
						Enemy[i].Speed.x = sin(Enemy[i].Angle * (PI / 180)) * ENEMY_SPEED;
						Enemy[i].Speed.y = -cos(Enemy[i].Angle * (PI / 180)) * ENEMY_SPEED;
					}
				}
				if (Enemy[i].MovePattern == 6)
				{
					if (Enemy[i].Age < 1200)
					{
						Enemy[i].Speed.y *= 0.99f;
					}
					else
					{
						Enemy[i].Speed.y -= 0.1f;
					}
				}
				if (Enemy[i].MovePattern == 7)
				{
					Enemy[i].Angle += 2;
					Enemy[i].Speed.x = cos(Enemy[i].Angle * (PI / 180)) * 4;
				}
				if (Enemy[i].MovePattern == 8)
				{
					if (Enemy[i].Age < 1800)
					{
						Enemy[i].Speed.y *= 0.98f;
					}
					else
					{
						Enemy[i].Speed.y -= 0.1f;
					}
				}
				if (Enemy[i].Speed.x > 1)
				{
					Enemy[i].Vector = 2;
				}
				else if(Enemy[i].Speed.x < -1)
				{
					Enemy[i].Vector = 1;
				}
				else
				{
					Enemy[i].Vector = 0;
				}
				Enemy[i].Position.x += Enemy[i].Speed.x;
				Enemy[i].Position.y += Enemy[i].Speed.y;
				Enemy[i].Collision.cx = Enemy[i].Position.x;
				Enemy[i].Collision.cy = Enemy[i].Position.y;
				Enemy[i].TextureAnime++;
				switch (Enemy[i].ShotPattern)
				{
				case 0://単発
					if (Enemy[i].Age % 240 == 60)
					{
						EBullet_Create(Enemy[i].Position.x, Enemy[i].Position.y, 0, 0);
						PlaySound(SOUND_LABEL_SE_E_SHOT);
					}
					break;
				case 1://5発
					if (Enemy[i].Age % 90 == 0)
					{
						EBullet_Create(Enemy[i].Position.x, Enemy[i].Position.y, 0, 1);
						PlaySound(SOUND_LABEL_SE_E_SHOT);
					}
					
					break;
				case 2://真下
					if (Enemy[i].Age % 120 == 30)
					{
						EBullet_Create(Enemy[i].Position.x, Enemy[i].Position.y, 1, 0);
						PlaySound(SOUND_LABEL_SE_E_SHOT);
					}
					break;
				case 3://大型
					if (Enemy[i].Age % 120 == 30)
					{
						EBullet_Create(Enemy[i].Position.x, Enemy[i].Position.y, 3, 0);
						PlaySound(SOUND_LABEL_SE_E_SHOT);
					}
					break;
				case 4://5連発
					if (Enemy[i].Age % 120 < 5)
					{
						EBullet_Create(Enemy[i].Position.x, Enemy[i].Position.y, 0, 0);
						PlaySound(SOUND_LABEL_SE_E_SHOT);
					}
					break;
				}
				Enemy[i].Age++;
			}
			else
			{
				Enemy[i].Position.x += Enemy[i].Speed.x;
				Enemy[i].Position.y -= Enemy[i].Speed.y;
				Enemy[i].Angle += 6;
				if (Enemy[i].Position.x < -ENEMY_WIDTH || Enemy[i].Position.x > SCREEN_WIDTH)
				{
					Enemy[i].Flag = false;
					Enemy[i].Dead = false;
				}
			}
			//弾が画面外だったら
			//無効にする
			if (Enemy[i].Position.y < -ENEMY_HEIGHT || Enemy[i].Position.y >= SCREEN_HEIGHT + ENEMY_HEIGHT ||
				Enemy[i].Position.x < -ENEMY_WIDTH  || Enemy[i].Position.x >= GAME_WIDTH    + ENEMY_WIDTH)
			{
				Enemy[i].Flag = false;
				Enemy[i].Dead = false;
			}
		}
	}
}
void Enemy_Draw(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//弾が有効だったら
		//弾の絵を描く
		if (Enemy[i].Flag)
		{
			if (Enemy[i].Drop)
			{
				if (Enemy[i].Dead)
				{
					Sprite_DrawRotation(g_TextureIndex[Enemy[i].Type], Enemy[i].Position.x, Enemy[i].Position.y, Enemy[i].TextureAnime * ENEMY_WIDTH, ENEMY_HEIGHT * 3, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_WIDTH, ENEMY_HEIGHT, 1, 1, Enemy[i].Angle);
				}
				else
				{
					Sprite_Draw(g_TextureIndex[Enemy[i].Type], Enemy[i].Position.x - (ENEMY_WIDTH / 2), Enemy[i].Position.y - (ENEMY_HEIGHT / 2), Enemy[i].TextureAnime * ENEMY_WIDTH, Enemy[i].Vector * ENEMY_HEIGHT, ENEMY_WIDTH, ENEMY_HEIGHT);
				}
			}
			else
			{
				if (Enemy[i].Dead)
				{
					Sprite_DrawRotation(g_TextureIndex[Enemy[i].Type], Enemy[i].Position.x, Enemy[i].Position.y, Enemy[i].TextureAnime * ENEMY_WIDTH, ENEMY_HEIGHT * 3, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_WIDTH, ENEMY_HEIGHT, 1, 1, Enemy[i].Angle);
				}
				else
				{
					Sprite_Draw(g_TextureIndex[Enemy[i].Type], Enemy[i].Position.x - (ENEMY_WIDTH / 2), Enemy[i].Position.y - (ENEMY_HEIGHT / 2), Enemy[i].TextureAnime * ENEMY_WIDTH, Enemy[i].Vector * ENEMY_HEIGHT, ENEMY_WIDTH, ENEMY_HEIGHT);
				}
			}
			DebugPrimitive_BatchDraw(Enemy[i].Collision.cx, Enemy[i].Collision.cy, Enemy[i].Collision.radius, D3DCOLOR_RGBA(0, 255, 0, 255));
		}
	}
}

void Enemy_Create(float x, float y, short move, short shot, short drop)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//弾が無効だったら
		//弾を有効にして
		//弾の座標をx,yにする
		if (Enemy[i].Flag == false)
		{
			Enemy[i].Flag = true;
			Enemy[i].Dead = false;
			Enemy[i].Position = D3DXVECTOR2(x, y);
			Enemy[i].Type = 0;
			Enemy[i].Angle = 0;
			Enemy[i].Drop = rand() % 2;
			if (Enemy[i].Drop)
			{
				Enemy[i].DropType = drop;
				Enemy[i].Type = 3;
			}
			Enemy[i].MovePattern = move;
			Enemy[i].ShotPattern = shot;
			switch (move)
			{
			case 0://右
				Enemy[i].Speed.x = ENEMY_SPEED;
				Enemy[i].Speed.y = 0;
				Enemy[i].Life = 10;
				break;
			case 1://左
				Enemy[i].Speed.x = -ENEMY_SPEED;
				Enemy[i].Speed.y = 0;
				Enemy[i].Life = 10;
				break;
			case 2://下
				Enemy[i].Speed.x = 0;
				Enemy[i].Speed.y = ENEMY_SPEED;
				Enemy[i].Life = 10;
				break;
			case 3://上
				Enemy[i].Speed.x = 0;
				Enemy[i].Speed.y = -ENEMY_SPEED;
				Enemy[i].Life = 10;
				break;
			case 4://上→右
				Enemy[i].Speed.x = 0;
				Enemy[i].Speed.y = -ENEMY_SPEED;
				Enemy[i].Life = 10;
				break;
			case 5://上→左
				Enemy[i].Speed.x = 0;
				Enemy[i].Speed.y = -ENEMY_SPEED;
				Enemy[i].Life = 10;
				break;
			case 6://下→停止
				Enemy[i].Speed.x = 0;
				Enemy[i].Speed.y = ENEMY_SPEED;
				Enemy[i].Life = 100;
				Enemy[i].Type = 1;
				Enemy[i].Drop = 1;
				break;
			case 7://フラフラ
				Enemy[i].Angle = rand() % 360;
				Enemy[i].Speed.x = 0;
				Enemy[i].Speed.y = ENEMY_SPEED / 2;
				Enemy[i].Life = 10;
				break;
			case 8://下→停止
				Enemy[i].Speed.x = 0;
				Enemy[i].Speed.y = ENEMY_SPEED;
				Enemy[i].Life = 300;
				Enemy[i].Type = 2;
				Enemy[i].Drop = 1;
				break;
			case 9://右下
				Enemy[i].Speed.x = rand();
				Enemy[i].Speed.y = rand();
				D3DXVec2Normalize(&Enemy[i].Speed, &Enemy[i].Speed);
				Enemy[i].Speed *= ENEMY_SPEED * 2;
				Enemy[i].Life = 20;
				break;
			case 10://左下
				Enemy[i].Speed.x = -rand();
				Enemy[i].Speed.y = rand();
				D3DXVec2Normalize(&Enemy[i].Speed, &Enemy[i].Speed);
				Enemy[i].Speed *= ENEMY_SPEED * 2;
				Enemy[i].Life = 20;
				break;
			
			}
			Enemy[i].Collision.cx = Enemy[i].Position.x;
			Enemy[i].Collision.cy = Enemy[i].Position.y;
			
			Enemy[i].TextureAnime = 0;
			
			Enemy[i].Age = 0;
			break;
		}
	}
}
int Enemy_Damage(int i)
{
	Enemy[i].Life--;
	if (Enemy[i].Life <= 0)
	{
		if (Enemy[i].Drop)
		{
			if (Enemy[i].Type == 1)
			{
				for (int j = 0; j < 10; j++)
				{
					Item_Create(Enemy[i].Position.x + (rand() % 81 - 40), Enemy[i].Position.y + (rand() % 81 - 40), Enemy[i].DropType);
				}
			}
			else if (Enemy[i].Type == 2)
			{
				for (int j = 0; j < 20; j++)
				{
					Item_Create(Enemy[i].Position.x + (rand() % 101 - 50), Enemy[i].Position.y + (rand() % 101 - 50), Enemy[i].DropType);
					Item_Create(Enemy[i].Position.x, Enemy[i].Position.y, 2);
				}
			}
			else
			{
				Item_Create(Enemy[i].Position.x, Enemy[i].Position.y, Enemy[i].DropType);
			}
		}
		Enemy[i].Dead = true;
		Enemy[i].Angle = rand() % 360;
		Enemy[i].Speed.x = cos(Enemy[i].Angle * (PI / 180)) * 10;
		Enemy[i].Speed.y = sin(Enemy[i].Angle * (PI / 180)) * 10;
		return 100;
	}
	return 1;

}
void Enemy_Destroy(int i)
{
	Enemy[i].Dead = true;
	
}
bool Enemy_IsEnable(int i)
{
	if(Enemy[i].Dead)
		return false;
	return Enemy[i].Flag;
}
const Circle* Enemy_GetCollision(int i)
{
	return &Enemy[i].Collision;
}
int Enemy_MaxGet(void)
{
	return ENEMY_MAX;
}
int Enemy_LifeGet(int i)
{
	return Enemy[i].Life;
}
int Enemy_GetFrame(void)
{
	return frame;
}