/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "enemyGenerator.h"
#include "judgement.h"
#include "explosion.h"
#include "texture.h"
#include "score.h"
#include "enemybullet.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "scene.h"
#include "input.h"
#include "background.h"
#include "anime.h"
#include "frame.h"
#include "item.h"
#include "option.h"
#include "bomb.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static bool g_fade;

void Game_Init()
{
	g_fade = false;
	Fade_Start(60, 0xff000000, false);
	Player_Init();
	Bullet_Init();
	Enemy_Init();
	Explosion_Init();
	EnemyGenerator_Init();
	Score_Init();
	EnemyBullet_Init();
	Effect_Init();
	Background_Init();
	Anime_Init();
	Frame_Init();
	Item_Init();
	Option_Init();
	Bomb_Init();

	Texture_Load();
	//PlaySound(SOUND_LABEL_BGM000);
}

void Game_Uninit()
{
	StopSound();
	Player_Uninit();
	Bullet_Uninit();
	Enemy_Uninit();
	Explosion_Uninit();
	EnemyGenerator_Uninit();
	EnemyBullet_Uninit();
	Effect_Uninit();
	Background_Uninit();
	Anime_Uninit();
	Frame_Uninit();
	Item_Uninit();
	Option_Uninit();
	Bomb_Uninit();
}

void Game_Update()
{
	
	if (!Fade_IsFade())
	{
		if (g_fade)
		{
			Scene_SetNextScene(SCENE_RESULT);
			return;
		}
		Player_Update();
		Bullet_Update();
		Enemy_Update();
		Explosion_Update();
		EnemyGenerator_Update();
		EnemyBullet_Update();
		Item_Update();
		Option_Update();
		Bomb_Update();
		Judgement_Update();
		Effect_Update();
		Background_Update();
		Anime_Update();
		
		if (Keyboard_IsPress(DIK_LSHIFT))
		{
			Fade_Start(60, 0x00000000, true);
			g_fade = true;
		}
	}
	
	
}

void Game_Draw()
{
	Background_Draw();
	Effect_Draw();
	Option_Draw();
	Item_Draw();
	EnemyBullet_Draw();
	Enemy_Draw();
	Bullet_Draw();
	Bomb_Draw();
	Player_Draw();
	Anime_Draw();
	Explosion_Draw();
	Frame_Draw();
	Score_Draw(GetScore(), 900, 96, 7);
	Score_Draw(Player_GetPower(), 948, 150, 3);
}