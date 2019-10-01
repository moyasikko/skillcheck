#ifndef COLLISION_H_
#define COLLISION_H_

typedef struct Circle_tag
{
	float cx, cy;
	float radius;
}Circle;

bool Collision_IsHitCircle(const Circle* pA, const Circle* pB);


#endif // !COLLISION_H_

