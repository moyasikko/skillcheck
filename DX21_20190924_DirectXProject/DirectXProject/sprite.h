#ifndef _SPRITE_H_
#define _SPRITE_H_

void SpriteInit(void);
void SpriteUninit(void);
void Sprite_Draw(int texId, float dx, float dy);
void Sprite_Draw(int texId, float dx, float dy, D3DCOLOR color);
void Sprite_DrawRotate(int texId, float dx, float dy, float angle);
void Sprite_DrawRotate(int texId, float dx, float dy, float angle, D3DCOLOR color);
void Sprite_DrawScale(int texId, float dx, float dy, float scale);
void Sprite_DrawScale(int texId, float dx, float dy, float scale, D3DCOLOR color);
void Sprite_DrawCut(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h);
void Sprite_DrawCut(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, D3DCOLOR color);
void Sprite_DrawReverse(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h);
void Sprite_DrawReverse(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, D3DCOLOR color);
void Sprite_DrawCR(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle);
void Sprite_DrawCR(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, D3DCOLOR color);
void Sprite_DrawCS(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float scale);
void Sprite_DrawCS(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float scale, D3DCOLOR color);
void Sprite_DrawRS(int texId, float dx, float dy, float angle, float scale);
void Sprite_DrawRS(int texId, float dx, float dy, float angle, float scale, D3DCOLOR color);
void Sprite_DrawCRS(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float scale);
void Sprite_DrawCRS(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float scale, D3DCOLOR color);
void Sprite_Draw(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float cx, float cy);
void Sprite_Draw(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float cx, float cy, D3DCOLOR color);

void Sprite_SetColor(D3DCOLOR color);

#endif // !_SPRITE_H_
