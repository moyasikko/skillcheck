#ifndef ITEM_H_
#define ITEM_H_

#include "collision.h"

void Item_Initialize(void);
void Item_Finalize(void);
void Item_Update(void);
void Item_Draw(void);

void Item_Create(float x, float y, int type);

void Item_Destroy(int i);
bool Item_IsEnable(int i);
const Circle* Item_GetCollision(int i);

int Item_MaxGet(void);
int Item_TypeGet(int i);
void Item_GetFlag(void);

#endif // !ITEM_H_
