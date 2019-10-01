#ifndef BOSS_H_
#define BOSS_H

#include "collision.h"

void Boss_Initialize(void);
void Boss_Finalize(void);
void Boss_Update(void);
void Boss_Draw(void);

int Boss_Damage(void);
void Boss_Destroy(void);

const Circle* Boss_GetCollision(void);
bool Boss_IsEnable();

#endif // !BOSS_H_
