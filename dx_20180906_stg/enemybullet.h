#ifndef ENEMYBULLET_H_
#define ENEMYBULLET_H_

#include "collision.h"

void EBullet_Initialize(void);
void EBullet_Finalize(void);
void EBullet_Update(void);
void EBullet_Draw(void);

void EBullet_Create(float x, float y, short type, short position);

void EBullet_Destroy(int i);
bool EBullet_IsEnable(int i);
const Circle* EBullet_GetCollision(int i);

int EBullet_MaxGet(void);

#endif