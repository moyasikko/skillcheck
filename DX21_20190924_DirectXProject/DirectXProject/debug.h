#ifndef _DEBUG_H_
#define _DEBUG_H_
#include <d3d9.h>

void Debug_Init(void);
void Debug_Uninit(void);
void Debug_Bigin(void);
void Debug_End(void);
void Debug_Draw(float x, float y, float radius);
void Debug_SetColor(D3DCOLOR color);

#endif // !_DEBUG_H_
