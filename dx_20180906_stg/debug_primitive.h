#ifndef DEBUG_PRIMITIVE_H_
#define DEBUG_PRIMITIVE_H_

#include <d3dx9.h>

void DebugPrimitive_Initialize(void);
void DebugPrimitive_Finalize(void);
void DebugPrimitive_BatchBegin(void);
void DebugPrimitive_BatchDraw(float cx, float cy, float radius, D3DCOLOR color);
void DebugPrimitive_BatchRun(void);

#endif // !DEBUG_PRIMITIVE_H_

