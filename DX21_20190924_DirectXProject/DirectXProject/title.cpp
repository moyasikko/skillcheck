/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "texture.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "input.h"
#include "scene.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_Texture;
static bool g_fade;

void Title_Init(void)
{
	g_fade = false;
	Fade_Start(60, 0xFF000000, false);
	g_Texture = Texture_SetLoadFile("./sozai/img/title.png", 1200, 900);
	Texture_Load();
}

void Title_Uninit(void)
{
	Texture_Destroy(&g_Texture, 1);
}

void Title_Update(void)
{
	if (Fade_IsFade())
	{

	}
	else
	{
		if (g_fade)
		{
			Scene_SetNextScene(SCENE_GAME);
		}
		if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsTrigger(XINPUT_GAMEPAD_A))
		{
			Fade_Start(60, 0x00000000, true);
			g_fade = true;
		}
	}
}

void Title_Draw(void)
{
	Sprite_Draw(g_Texture, 0, 0);
}