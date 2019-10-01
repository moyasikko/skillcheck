#ifndef BULLET_H_
#define BULLET_H_

#include "collision.h"

void Bullet_Initialize(void);
void Bullet_Finalize(void);
void Bullet_Update(void);
void Bullet_Draw(void);

void Bullet_Create(float x, float y, short type, short position);

void Bullet_Destroy(int i,int j);
bool Bullet_IsEnable(int i);
const Circle* Bullet_GetCollision(int i);

int Bullet_MaxGet(void);

#endif // !BULLET_H_
