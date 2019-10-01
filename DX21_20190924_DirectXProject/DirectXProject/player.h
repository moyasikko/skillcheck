#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "collision.h"

void Player_Init(void);
void Player_Uninit(void);
void Player_Update(void);
void Player_Draw(void);

const Circle* Player_GetCollision(void);
void Player_Damege(void);
bool Player_GetFlag(void);
D3DXVECTOR2 Player_GetPosition(void);
void Player_PowerUp(void);
int Player_GetPower(void);

#endif // !_PLAYER_H_
