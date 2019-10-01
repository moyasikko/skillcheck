/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "game.h"
#include "texture.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "judgement.h"
#include "score.h"
#include "common.h"
#include "number.h"
#include "item.h"
#include "option.h"
#include "Debugfont.h"
#include "fade.h"
#include "effect.h"
#include "enemybullet.h"
#include "input.h"
#include "scene.h"
#include "frame.h"
#include "anime.h"
#include "background.h"
#include "boss.h"
#include "warnig.h"
#include "explosion.h"
#include "sound.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static bool g_bIsFade = false;
static int frame;
static bool boss;
static bool warning;
static bool clear;

void Game_Initialize(void)
{
	frame = 0;
	boss = false;
	warning = false;
	clear = false;
	Player_Initialize();
	Bullet_Initialize();
	Enemy_Initialize();
	Item_Initialize();
	Option_Initialize();
	Score_Initialize(8);
	Judgement_Initialize();
	Effect_Initialize();
	EBullet_Initialize();
	Frame_Initialize();
	Anime_Initialize();
	Background_Initialize();
	Boss_Initialize();
	Explosion_Initialize();
	Warning_Initialize();
	
	if (Texture_Load() > 0)
	{
		return;
	}
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 255));
	PlaySound(SOUND_LABEL_SE_ITAZURA);
}
void Game_Finalize(void)
{
	StopSound();
	Player_Finalize();
	Bullet_Finalize();
	Enemy_Finalize();
	Item_Finalize();
	Score_Finalize();
	Option_Finalize();
	Effect_Finalize();
	EBullet_Finalize();
	Frame_Finalize();
	Anime_Finalize();
	Background_Finalize();
	Boss_Finalize();
	Warning_Finalize();
	Explosion_Finalize();
	Texture_AllRelease();
	//if (_DEBUG)
		//DebugFont_Finalize();
}
void Game_Update(void)
{
	frame++;
	Player_Update();
	Option_Update();
	Bullet_Update();
	if(boss)
		Boss_Update();
	Enemy_Update();
	Item_Update();
	EBullet_Update();
	Judgement_Update();
	Effect_Update();
	Anime_Update();
	Explosion_Update();
	Background_Update();
	if (Enemy_GetFrame() == 10000)
	{
		StopSound();
		warning = true;
		PlaySound(SOUND_LABEL_SE_WARNING);
	}
	if (warning)
		Warning_Update();
	if (Warning_GetFrame() == 250)
	{
		warning = false;
		boss = true;
	}
	if (!g_bIsFade) {
		if (Player_Life() == 0 || clear)
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
			Scene_Change(SCENE_INDEX_RESULT);
		}
	}
}
void Game_Draw(void)
{
	Background_Draw();
	
	Item_Draw();
	Effect_Draw();
	EBullet_Draw();
	if(boss)
		Boss_Draw();
	Enemy_Draw();
	Player_Draw();
	Bullet_Draw();
	Option_Draw();
	Explosion_Draw();

	if (warning)
		Warning_Draw();

	Frame_Draw();
	Anime_Draw();
	Score_Draw(SCREEN_WIDTH - NUMBER_WIDTH * 10, NUMBER_HEIGHT, Score(), false, false);
	Power_Draw(SCREEN_WIDTH - NUMBER_WIDTH * 2, NUMBER_HEIGHT * 3, Player_Power(), false, false);
	Life_Draw(SCREEN_WIDTH - NUMBER_WIDTH * 2, NUMBER_HEIGHT * 5, Player_Life(), false, false);
	//if(_DEBUG)
	//	DebugFont_Draw();
}

void Game_Clear(void)
{
	boss = false;
	clear = true;
}

bool Game_GetClear(void)
{
	return clear;
}