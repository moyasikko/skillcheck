/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "texture.h"
#include "sprite.h"
#include "common.h"
/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_TextureIndex = { TEXTURE_INVALID_INDEX };
static float scroll;

void Background_Initialize(void)
{
	g_TextureIndex = Texture_SetLoadFile("./img/background01.png", 512, 800);
	scroll = 0;
}
void Background_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_TextureIndex, 1);
}
void Background_Update(void)
{
	scroll--;
}
void Background_Draw(void)
{
	Sprite_Draw(g_TextureIndex, 0, 0, 6, scroll, GAME_WIDTH, SCREEN_HEIGHT);
}