#ifndef _EFFECT_H_
#define _EFFECT_H_
#include <d3d9.h>

#define EFFECT_MAX (8192)

void Effect_Init(void);
void Effect_Uninit(void);
void Effect_Update(void);
void Effect_Draw(void);

void Effect_Create(float x, float y, float size, D3DCOLOR color, int lifeframe);

#endif // !_EFFECT_H_
