/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "scene.h"
#include "input.h"
#include "fade.h"
#include "texture.h"
#include "sprite.h"
#include "common.h"
#include "sound.h"


static bool g_bIsFade = false;
static int g_TextureIndex[8] = { TEXTURE_INVALID_INDEX };
static float scroll_a, scroll_b, scroll_c;
static bool bgm;

void Title_Initialize(void)
{
	bgm = false;
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 255));
	g_TextureIndex[0] = Texture_SetLoadFile("./img/title.png", 800, 600);
	g_TextureIndex[1] = Texture_SetLoadFile("./img/titlelogo.png", 640, 300);
	g_TextureIndex[2] = Texture_SetLoadFile("./img/roop_a.png", 76, 92);
	g_TextureIndex[3] = Texture_SetLoadFile("./img/roop_b.png", 228, 276);
	g_TextureIndex[4] = Texture_SetLoadFile("./img/roop_c.png", 304, 368);
	g_TextureIndex[5] = Texture_SetLoadFile("./img/push_space.png", 300, 30);
	g_TextureIndex[6] = Texture_SetLoadFile("./img/osusume.png", 100, 100);
	if (Texture_Load() > 0)
	{
		return;
	}
	scroll_a = scroll_b = scroll_c = 0;
	
}
void Title_Finalize(void)
{
	Texture_Release(&g_TextureIndex[0], 7);
	StopSound();
}
void Title_Update(void)
{
	if (bgm == false)
	{
		PlaySound(SOUND_LABEL_SE_TITLE);
		bgm = true;
	}
	if (!g_bIsFade) {
		if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsTrigger(XINPUT_GAMEPAD_A))
		{
			Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 255));
			g_bIsFade = true;
		}
	}
	else
	{
		if (!Fade_IsFade())
		{
			g_bIsFade = false;
			Scene_Change(SCENE_INDEX_GAME);
		}
	}
	scroll_a++;
	scroll_b -= 0.75f;
	scroll_c += 0.5f;
}
void Title_Draw(void)
{
	Sprite_Draw(g_TextureIndex[0], 0, 0);
	Sprite_Draw(g_TextureIndex[4], 0, SCREEN_HEIGHT -500, scroll_c, 0, SCREEN_WIDTH, 368);
	Sprite_Draw(g_TextureIndex[3], 0, SCREEN_HEIGHT -300, scroll_b, 0, SCREEN_WIDTH, 276);
	Sprite_Draw(g_TextureIndex[2], 0, SCREEN_HEIGHT -100, scroll_a, 0, SCREEN_WIDTH, 92);


	Sprite_Draw(g_TextureIndex[1], 80, 0);
	Sprite_Draw(g_TextureIndex[5], 250, 400);

	Sprite_Draw(g_TextureIndex[6], 695, 5);
}