#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include <d3dx9.h>

void Explosion_Initialize(void);
void Explosion_Finalize(void);
void Explosion_Update(void);
void Explosion_Draw(void);

void Explosion_Create(D3DXVECTOR2);

#endif // !EXPLOSION_H_

