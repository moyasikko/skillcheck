/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "texture.h"
#include "sprite.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define COCO_WALK_R_PATTERN_MAX	(10)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
int g_Texture;

bool PatternAnimation_Init(HWND hWnd)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/runningman102.png", 700, 400);
	int error = Texture_Load();
	if (error > 0)
	{
		MessageBox(hWnd, "なんかミスってるよ（笑）", "g_pTexture", MB_OK);
		return false;
	}
	return true;
}
void PatternAnimation_Draw(float dx, float dy, int frameCount, bool left)
{
	int pattern = frameCount / 4 % COCO_WALK_R_PATTERN_MAX;
	if (left)
	{
		Sprite_DrawCut(g_Texture, dx, dy, (pattern % 5) * 140, (pattern / 5) * 200, 140, 200);
	}
	else
	{
		Sprite_DrawReverse(g_Texture, dx, dy, (pattern % 5) * 140, (pattern / 5) * 200, 140, 200);
	}
}