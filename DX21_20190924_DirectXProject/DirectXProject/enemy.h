#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "collision.h"
#include "item.h"

#define ENEMY_MAX (64)
#define ENEMY_SIZE	(62)
enum ENEMY_TYPE
{
	TYPE1,//��
	TYPE2,//��
	TYPE3,//��
	TYPE4,//��
	TYPE_MAX
};
enum ENEMY_MOVETYPE
{
	MOVETYPE1,//��
	MOVETYPE2,//��
	MOVETYPE3,//�E
	MOVETYPE4,//��
	MOVETYPE5,//�t���t��
	MOVETYPE6,//������~
	MOVETYPE7,//�E��
	MOVETYPE8,//����
	MOVETYPE_MAX
};

enum ENEMY_SHOTTYPE
{
	SHOTTYPE1,//��l���_��
	SHOTTYPE2,//�^������
	SHOTTYPE3,//�E��]
	SHOTTYPE4,//����]
	SHOTTYPE5,//6����
	SHOTTYPE_MAX
};

enum ENEMY_BULLETTYPE
{
	BULLETTYPE1,	//��l���_��
	BULLETTYPE2,	//�܂�����
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
