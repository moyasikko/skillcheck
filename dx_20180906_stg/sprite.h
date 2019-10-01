#ifndef SPRITE_H_
#define SPRITE_H_

#include <d3dx9.h>
/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void Sprite_Initialize(void);
void Sprite_Finalize(void);
void Sprite_SetColor(D3DCOLOR color);
void Sprite_Draw(int texture_index, float dx, float dy);
void Sprite_DrawRotate(int texture_index, float dx, float dy, float f);
void Sprite_Draw(int texture_index, float dx, float dy, int tx, int ty, int tw, int th);
void Sprite_DrawRotation(int texture_index, float dx, float dy, int tx, int ty, int tw, int th, float cx, float cy, float sx, float sy, float rotation);


#endif
