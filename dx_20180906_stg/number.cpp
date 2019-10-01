/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "number.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include <d3dx9.h>

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_NumberTextureIndex = TEXTURE_INVALID_INDEX;

void Number_Initialize(void)
{
	g_NumberTextureIndex = Texture_SetLoadFile("./img/number.png", 240, 24);
}
void Number_Finalize(void)
{
	//テクスチャーの解放
	Texture_Release(&g_NumberTextureIndex, 1);
}
void Number_Draw(float x, float y, int n, float size)
{
	Sprite_DrawRotation(g_NumberTextureIndex, x + NUMBER_WIDTH / 2, y + NUMBER_HEIGHT / 2, n * NUMBER_WIDTH, 0, NUMBER_WIDTH, NUMBER_HEIGHT, 0, 0, size, size, 0);
}