#ifndef _BULLET_H_
#define _BULLET_H_
#include "collision.h"

#define BULLET_MAX (1024)

void Bullet_Init(void);
void Bullet_Uninit(void);
void Bullet_Update(void);
void Bullet_Draw(void);

void Bullet_Create(float x, float y);
bool Bullet_Flag(int i);
void Bullet_Destroy(int i);
const Circle* Bullet_GetCollision(int i);

#endif // !_BULLET_H_
