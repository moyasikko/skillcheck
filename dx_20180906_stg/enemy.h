#ifndef ENEMY_H_
#define ENEMY_H_

#include "collision.h"

void Enemy_Initialize(void);
void Enemy_Finalize(void);
void Enemy_Update(void);
void Enemy_Draw(void);

void Enemy_Create(float x, float y, short move, short shot, short drop);
int Enemy_Damage(int i);
void Enemy_Destroy(int i);
bool Enemy_IsEnable(int i);
const Circle* Enemy_GetCollision(int i);

int Enemy_MaxGet(void);
int Enemy_LifeGet(int i);
int Enemy_GetFrame(void);

#endif 