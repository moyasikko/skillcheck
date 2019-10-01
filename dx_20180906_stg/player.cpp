/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "player.h"
#include "common.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "bullet.h"
#include "effect.h"
#include "sound.h"
#include "anime.h"
#include "fade.h"
#include "item.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define PLAYER_WIDTH		(50)
#define PLAYER_HEIGHT		(50)
#define PLAYER_MOVE_SPEED	(10.0f)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static int g_atariTextureIndex = TEXTURE_INVALID_INDEX;
static D3DXVECTOR2 g_Position(0.0f, 0.0f);
static D3DXVECTOR2 g_Speed(0.0f, 0.0f);
static int interval = 0;
static int power;
static Circle g_PlayerCollision;
static int PlayerLife;
static int mutekicount;
static short muki;
static int PlayerLifeOld;


void Player_Initialize(void)
{
	g_TextureIndex = Texture_SetLoadFile("./img/player.png", 150, 50);
	g_atariTextureIndex = Texture_SetLoadFile("./img/atari.png", 10, 10);
	g_Position.x = GAME_WIDTH / 2;
	g_Position.y = SCREEN_HEIGHT - PLAYER_HEIGHT / 2;
	g_PlayerCollision.cx = g_Position.x;
	g_PlayerCollision.cy = g_Position.y;
	g_PlayerCollision.radius = 1;
	g_Speed = D3DXVECTOR2(0.0f, 0.0f);
	power = 0;
	PlayerLife = 10;
	mutekicount = 240;
	muki = 1;
	PlayerLifeOld = PlayerLife;
}
void Player_Finalize(void)
{
	Texture_Release(&g_TextureIndex, 1);
}
void Player_Update(void)
{
	muki = 1;
	if (mutekicount > 0)
	{
		mutekicount--;
		if (mutekicount == 120)
		{
			GamePad_VibrationStop();
		}
	}
	if (PlayerLife <= 0)
		return;
	if (Fade_IsFade())
		return;
	D3DXVECTOR2 vecDir(0.0f, 0.0f);
	if (g_Position.x <= GAME_WIDTH - (PLAYER_WIDTH * 0.5f))
		if (Keyboard_IsPress(DIK_D) || Keyboard_IsPress(DIK_RIGHTARROW))
		{
			vecDir.x += 1.0f;
			muki--;
		}
	if (g_Position.x >= 0 + (PLAYER_WIDTH * 0.5f))
		if (Keyboard_IsPress(DIK_A) || Keyboard_IsPress(DIK_LEFTARROW))
		{
			vecDir.x -= 1.0f;
			muki++;
		}
	if (g_Position.y >= 0 + (PLAYER_HEIGHT * 0.5f))
		if (Keyboard_IsPress(DIK_W) || Keyboard_IsPress(DIK_UPARROW))
		{
			vecDir.y -= 1.0f;
		}
	if (g_Position.y <= SCREEN_HEIGHT - (PLAYER_HEIGHT * 0.5f))
		if (Keyboard_IsPress(DIK_S) || Keyboard_IsPress(DIK_DOWNARROW))
		{
			vecDir.y += 1.0f;
		}
	vecDir = GamePad_GetThumbL();
	
	D3DXVec2Normalize(&vecDir, &vecDir);
	if (Keyboard_IsPress(DIK_M) || Keyboard_IsPress(DIK_LSHIFT) || GamePad_IsPress(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		g_Speed += vecDir * PLAYER_MOVE_SPEED / 2;
	}
	else
	{
		g_Speed += vecDir * PLAYER_MOVE_SPEED;
	}
	g_Position += g_Speed;
	if (g_Speed.x > 0)
	{
		muki--;
	}
	else if (g_Speed.x < 0)
	{
		muki++;
	}
	g_Speed *= 0;
	

	if (g_Position.x < 0 + (PLAYER_WIDTH * 0.5f))
		g_Position.x = PLAYER_WIDTH * 0.5f;
	if (g_Position.x > GAME_WIDTH - (PLAYER_WIDTH * 0.5f))
		g_Position.x = GAME_WIDTH - (PLAYER_WIDTH * 0.5f);
	if (g_Position.y < 0 + (PLAYER_HEIGHT * 0.5f))
		g_Position.y = PLAYER_HEIGHT * 0.5f;
	if (g_Position.y > SCREEN_HEIGHT - (PLAYER_HEIGHT * 0.5f))
		g_Position.y = SCREEN_HEIGHT - (PLAYER_HEIGHT * 0.5f);

	//プレイヤー座標が確定した
	interval--;
	if (interval <= 0)
	{
		if (Keyboard_IsPress(DIK_SPACE) || Keyboard_IsPress(DIK_Z) || GamePad_IsPress(XINPUT_GAMEPAD_A))
		{
			interval = 3;
			Bullet_Create(g_Position.x, g_Position.y, 0, 0);
			PlaySound(SOUND_LABEL_SE_SHOT);
		}
	}
	
	g_PlayerCollision.cx = g_Position.x;
	g_PlayerCollision.cy = g_Position.y;

	if (g_Position.y <= ITEM_LINE)
		Item_GetFlag();
	//Effect_Create(g_Position.x, g_Position.y, 40, 100, 0x88FF0000);
	//Effect_Create(g_Position.x, g_Position.y, 40,  50, 0x88FFFFFF);
	PlayerLifeOld = PlayerLife;
}
void Player_Draw(void)
{
	if (mutekicount % 2 == 0)
	{
		//Sprite_Draw(g_TextureIndex, g_Position.x - PLAYER_WIDTH * 0.5f, g_Position.y - PLAYER_HEIGHT * 0.5f);
		Sprite_Draw(g_TextureIndex, g_Position.x - (PLAYER_WIDTH / 2), g_Position.y - (PLAYER_HEIGHT / 2), muki * PLAYER_WIDTH, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
	}
	if (Keyboard_IsPress(DIK_M) || Keyboard_IsPress(DIK_LSHIFT) || GamePad_IsPress(XINPUT_GAMEPAD_LEFT_SHOULDER))
		Sprite_Draw(g_atariTextureIndex, g_Position.x - 5, g_Position.y - 5);
}

const Circle* Player_GetCollision(void)
{
	return &g_PlayerCollision;
}

int Player_Powerup(void)
{
	if (power < 500)
	{
		power += 5;
		if(power%100==0)
			PlaySound(SOUND_LABEL_SE_POWERUP);
		return 0;
	}
	return 1000;
}

float Player_Power(void)
{
	return power;
}

D3DXVECTOR2 Player_GetPosition(void)
{
	return g_Position;
}

void Player_Damage(void)
{
	PlayerLife--;
	mutekicount = 180;
	PlaySound(SOUND_LABEL_SE_DAMAGE);
	Anime_Create(g_Position);
	GamePad_VibrationStart();
}
int Player_Life(void)
{
	return PlayerLife;
}

int Muteki(void)
{
	return mutekicount;
}

void Player_LifeUp(void)
{
	if (PlayerLifeOld == PlayerLife)
	{
		PlayerLife++;
		PlaySound(SOUND_LABEL_SE_POWERUP);
	}
}