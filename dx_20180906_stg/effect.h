#ifndef EFFECT_H_
#define EFFECT_H_

#include <d3dx9.h>

void Effect_Initialize();
void Effect_Finalize();
void Effect_Update();
void Effect_Draw();

void Effect_Create(float x, float y, int life, float radius, D3DCOLOR color);

#endif // !EFFECT_H_

