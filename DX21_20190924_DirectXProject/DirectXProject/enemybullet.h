#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_
#include "collision.h"
#include "enemy.h"

#define ENEMYBULLET_MAX (2048)

void EnemyBullet_Init(void);
void EnemyBullet_Uninit(void);
void EnemyBullet_Update(void);
void EnemyBullet_Draw(void);

void EnemyBullet_Create(float x, float y, ENEMY_BULLETTYPE type, int option);
bool EnemyBullet_Flag(int i);
void EnemyBullet_Destroy(int i);
const Circle* EnemyBullet_GetCollision(int i);

#endif // !_ENEMYBULLET_H_
