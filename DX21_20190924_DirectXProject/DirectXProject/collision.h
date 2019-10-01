#ifndef _COLLISION_H_
#define _COLLISION_H_
#include <d3dx9.h>

typedef struct Circle_tag
{
	D3DXVECTOR2 position;
	float radius;
}Circle;

bool Circle_Circle(const Circle *pA, const Circle *pB);

#endif // !_COLLISION_H_
