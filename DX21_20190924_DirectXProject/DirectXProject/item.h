#ifndef _ITEM_H_
#define _ITEM_H_
#include "collision.h"

enum ITEM_TYPE
{
	ITEMTYPE1,		//�p���[
	ITEMTYPE2,		//�X�R�A
	ITEMTYPE3,		//���C�t
	ITEMTYPE_MAX	//
};

#define ITEM_MAX (128)

void Item_Init(void);
void Item_Uninit(void);
void Item_Update(void);
void Item_Draw(void);

void Item_Create(float x, float y, ITEM_TYPE type);
bool Item_Flag(int i);
void Item_Destroy(int i);
const Circle* Item_GetCollision(int i);

#endif // !_ITEM_H_
