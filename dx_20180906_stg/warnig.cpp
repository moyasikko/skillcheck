/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "texture.h"
#include "sprite.h"
#include "common.h"
/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_TextureIndex[3] = { TEXTURE_INVALID_INDEX };
static float scroll;
static int frame;
static int slide;

void Warning_Initialize(void)
{
	g_TextureIndex[0] = Texture_SetLoadFile("./img/warning!.png", 400, 50);
	g_TextureIndex[1] = Texture_SetLoadFile("./img/warningback.png", 100, 100);
	g_TextureIndex[2] = Texture_SetLoadFile("./img/kiken.png", 64, 16);
	scroll = 0;
	frame = 0;
	slide = 500;
}
void Warning_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_TextureIndex[0], 3);
}
void Warning_Update(void)
{
	if (frame < 50 || frame > 200)
		slide -= 10;
	scroll++;
	frame++;
}
void Warning_Draw(void)
{
	Sprite_Draw(g_TextureIndex[1], slide + 0 , 250, 0, 0, GAME_WIDTH, 100);
	Sprite_Draw(g_TextureIndex[0], slide + 50, 275, 0, 0, 400, 50);
	Sprite_Draw(g_TextureIndex[2], slide + 0 , 255, scroll, 0, GAME_WIDTH, 16);
	Sprite_Draw(g_TextureIndex[2], slide + 0 , 330, -scroll, 0, GAME_WIDTH, 16);
}

int Warning_GetFrame(void)
{
	return frame;
}