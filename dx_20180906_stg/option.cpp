/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "bullet.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "input.h"
#include "fade.h"


/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define OPTION_SIZE		(45)
#define OPTION_MAX		(5)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_OptionTextureIndex = TEXTURE_INVALID_INDEX;
static D3DXVECTOR2 g_Position[OPTION_MAX];
static float angle[OPTION_MAX] = { 0 };
static int interval = 0;
static short position[OPTION_MAX];



void Option_Initialize(void)
{
	g_OptionTextureIndex = Texture_SetLoadFile("./img/star.png", OPTION_SIZE, OPTION_SIZE);
}
void Option_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_OptionTextureIndex, 1);
}
void Option_Update(void)
{
	if (Fade_IsFade())
		return;
	int power = Player_Power() / 100;
	for (int i = 0; i < power; i++)
	{
		g_Position[i] = Player_GetPosition();
		angle[i] += 5.0f;
	}
	switch (power)
	{
	case 1:
		g_Position[0].y += 50.0f;
		position[0] = 0;
		break;
	case 2:
		g_Position[0].x += 50.0f;
		position[0] = 1;
		g_Position[1].x -= 50.0f;
		position[1] = 2;
		break;
	case 3:
		g_Position[0].x += 50.0f;
		position[0] = 1;
		g_Position[1].x -= 50.0f;
		position[1] = 2;
		g_Position[2].y += 50.0f;
		position[2] = 0;
		break;
	case 4:
		g_Position[0].x += 60.0f;
		position[0] = 3;
		g_Position[1].x -= 60.0f;
		position[1] = 4;
		g_Position[2].x += 30.0f;
		g_Position[2].y += 30.0f;
		position[2] = 1;
		g_Position[3].x -= 30.0f;
		g_Position[3].y += 30.0f;
		position[3] = 2;
		break;
	case 5:
		g_Position[0].x += 30.0f;
		g_Position[0].y += 30.0f;
		position[0] = 1;
		g_Position[1].x -= 30.0f;
		g_Position[1].y += 30.0f;
		position[1] = 2;
		g_Position[2].x += 60.0f;
		position[2] = 3;
		g_Position[3].x -= 60.0f;
		position[3] = 4;
		g_Position[4].y += 60.0f;
		position[4] = 0;
		break;
	}

	interval--;
	if (interval <= 0)
	{
		if (Keyboard_IsPress(DIK_SPACE) || Keyboard_IsPress(DIK_Z) || GamePad_IsPress(XINPUT_GAMEPAD_A))
		{
			interval = 3;
			for (int i = 0; i < power; i++)
			Bullet_Create(g_Position[i].x, g_Position[i].y, 1, position[i]);
		}
	}
}
void Option_Draw(void)
{
	int power = Player_Power() / 100;
	for (int i = 0; i < power; i++)
	{
		Sprite_DrawRotate(g_OptionTextureIndex, g_Position[i].x, g_Position[i].y, angle[i]);
	}
}