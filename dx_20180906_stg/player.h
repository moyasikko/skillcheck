#ifndef PLAYER_H_
#define PLAYER_H_

#include "collision.h"
#include <d3dx9.h>

void Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
const Circle* Player_GetCollision(void);
int Player_Powerup(void);
float Player_Power(void);
D3DXVECTOR2 Player_GetPosition(void);
void Player_Damage(void);
int Player_Life(void);
int Muteki(void);
void Player_LifeUp(void);

#endif