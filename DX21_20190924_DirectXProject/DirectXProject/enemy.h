#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "collision.h"
#include "item.h"

#define ENEMY_MAX (64)
#define ENEMY_SIZE	(62)
enum ENEMY_TYPE
{
	TYPE1,//緑
	TYPE2,//青
	TYPE3,//赤
	TYPE4,//黒
	TYPE_MAX
};
enum ENEMY_MOVETYPE
{
	MOVETYPE1,//下
	MOVETYPE2,//上
	MOVETYPE3,//右
	MOVETYPE4,//左
	MOVETYPE5,//フラフラ
	MOVETYPE6,//下→停止
	MOVETYPE7,//右下
	MOVETYPE8,//左下
	MOVETYPE_MAX
};

enum ENEMY_SHOTTYPE
{
	SHOTTYPE1,//主人公狙い
	SHOTTYPE2,//真っすぐ
	SHOTTYPE3,//右回転
	SHOTTYPE4,//左回転
	SHOTTYPE5,//6放射
	SHOTTYPE_MAX
};

enum ENEMY_BULLETTYPE
{
	BULLETTYPE1,	//主人公狙い
	BULLETTYPE2,	//まっすぐ
	BULLETTYPE3,	//
	BULLETTYPE4,	//
	BULLETTYPE5,	//
	BULLETTYPE_MAX	//
};

void Enemy_Init(void);
void Enemy_Uninit(void);
void Enemy_Update(void);
void Enemy_Draw(void);

void Enemy_Create(float x, float y, int life, ENEMY_TYPE type, ENEMY_MOVETYPE move, ENEMY_SHOTTYPE shot, ITEM_TYPE item);
bool Enemy_Flag(int i);
void Enemy_Damage(int i);
void Enemy_Destroy(int i);
const Circle* Enemy_GetCollision(int i);
D3DXVECTOR2 Enemy_GetPosition(int i);

#endif // !_ENEMY_H_
