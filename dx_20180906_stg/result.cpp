/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "scene.h"
#include "input.h"
#include "fade.h"
#include "texture.h"
#include "sprite.h"
#include "judgement.h"
#include "score.h"
#include "common.h"
#include "number.h"
#include "sound.h"
#include "game.h"
#include <stdio.h>



static bool g_bIsFade = false;
static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static int g_NewTextureIndex = TEXTURE_INVALID_INDEX;
static int g_Score;
static int ranking_score[RANKING];
static int work;
static int rank;
static int count;

void Result_Initialize(void)
{
	count = 0;
	if (Game_GetClear())
	{
		PlaySound(SOUND_LABEL_SE_SUCCESS);
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_GAMEOVER);
	}
	FILE *fp;
	//fp = fopen("time.txt", "rt");
	g_Score = Score();
	//fscanf(fp, "%d", &g_Score);
	//fclose(fp);
	fp = fopen("score.txt", "rt");
	

	for (int i = 0; i < RANKING; i++)
	{
		fscanf(fp, "%d", &ranking_score[i]);
	}
	fclose(fp);

	rank = RANKING + 1;
	for (int i = RANKING - 1; i >= 0; i--)
	{
		if (g_Score >= ranking_score[i])
		{
			rank--;
		}
		else
			break;
	}
	for (int i = 0; i < RANKING; i++)
	{
		if (g_Score >= ranking_score[i])
		{
			for (i; i < RANKING; i++)
			{
				work = ranking_score[i];
				ranking_score[i] = g_Score;
				g_Score = work;
			}
		}
	}
	fp = fopen("score.txt", "wt");
	fprintf(fp, "%d %d %d %d %d %d %d %d %d %d", ranking_score[0], ranking_score[1], ranking_score[2], ranking_score[3], ranking_score[4], ranking_score[5], ranking_score[6], ranking_score[7], ranking_score[8], ranking_score[9]);
	fclose(fp);
	Score_Initialize(8);
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 255));
	g_TextureIndex = Texture_SetLoadFile("./img/result.png", 800, 600);
	g_NewTextureIndex = Texture_SetLoadFile("./img/new.png", 72, 288);
	if (Texture_Load() > 0)
	{
		return;
	}
}
void Result_Finalize(void)
{
	Score_Finalize();
	Texture_Release(&g_TextureIndex, 1);
}
void Result_Update(void)
{
	count++;
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
			Scene_Change(SCENE_INDEX_TITLE);
		}
	}
}
void Result_Draw(void)
{
	Sprite_Draw(g_TextureIndex, 0, 0);
	Score_Draw(NUMBER_WIDTH * 5, NUMBER_HEIGHT * 3, Score(), false, false);
	ScoreRanking_Draw(SCREEN_WIDTH - NUMBER_WIDTH * 13, NUMBER_HEIGHT * 3, ranking_score, false, false);
	if(rank < RANKING + 1)
		Sprite_Draw(g_NewTextureIndex, SCREEN_WIDTH - NUMBER_WIDTH * 16,
			NUMBER_HEIGHT * 3 + (rank - 1) * NUMBER_HEIGHT, 
			0, (rank - 1 + count) * 24,
			72, 24);

}