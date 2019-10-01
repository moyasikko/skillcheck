/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "collision.h"

bool Circle_Circle(const Circle *pA, const Circle *pB)
{
	return (pA->radius + pB->radius)*(pA->radius + pB->radius) > D3DXVec2LengthSq(&(pA->position - pB->position));
}