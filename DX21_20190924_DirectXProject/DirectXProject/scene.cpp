/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static SCENE g_Scene = SCENE_TITLE;
static SCENE g_NextScene = SCENE_TITLE;

typedef void(*Func)(void);
static const Func INITS[] = { Title_Init,Game_Init,Result_Init };
static const Func UNINITS[] = { Title_Uninit,Game_Uninit,Result_Uninit };
static const Func UPDATES[] = { Title_Update,Game_Update,Result_Update };
static const Func DRAWS[] = { Title_Draw,Game_Draw,Result_Draw };

void Scene_Init(void)
{
	INITS[g_NextScene]();
}

void Scene_Uninit(void)
{
	UNINITS[g_Scene]();
}

void Scene_Update(void)
{
	UPDATES[g_Scene]();
}

void Scene_Draw(void)
{
	DRAWS[g_Scene]();
}

void Scene_SetNextScene(SCENE next_scene)
{
	g_NextScene = next_scene;
}

bool Scene_Change(void)
{
	if (g_NextScene != g_Scene)
	{
		Scene_Uninit();
		if (g_NextScene == SCENE_END)
		{
			return true;
		}
		
		Scene_Init();
		g_Scene = g_NextScene;
	}
	return false;
}