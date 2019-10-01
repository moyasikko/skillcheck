/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "number.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static int g_Texture;


void Number_Init(void)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/number.png", NUMBER_SIZE * 10, NUMBER_SIZE);
}

void Number_Draw(int number, float x, float y)
{
	if (number < 0 || number>9)
	{
		return;
	}
	Sprite_DrawCut(g_Texture, x + (NUMBER_SIZE / 2), y + (NUMBER_SIZE / 2), NUMBER_SIZE * number, 0, NUMBER_SIZE, NUMBER_SIZE);
}