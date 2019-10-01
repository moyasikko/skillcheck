/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "collision.h"

bool Collision_IsHitCircle(const Circle* pA, const Circle* pB)
{
	float abr = pA->radius + pB->radius;
	float ll = (pA->cx - pB->cx) * (pA->cx - pB->cx) + (pA->cy - pB->cy) * (pA->cy - pB->cy);

	return abr * abr > ll;
}