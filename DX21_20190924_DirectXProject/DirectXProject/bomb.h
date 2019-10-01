#ifndef _BOMB_H_
#define _BOMB_H_
#include "collision.h"

void Bomb_Init(void);
void Bomb_Uninit(void);
void Bomb_Update(void);
void Bomb_Draw(void);

void Bomb_Create(float x, float y);
bool Bomb_Flag(void);
const Circle* Bomb_GetCollision(void);

#endif // !_BOMB_H_
