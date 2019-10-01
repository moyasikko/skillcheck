#ifndef _SCENE_H_
#define _SCENE_H_

enum SCENE
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_END,
	SCENE_MAX
};

void Scene_Init(void);
void Scene_Uninit(void);
void Scene_Update(void);
void Scene_Draw(void);

void Scene_SetNextScene(SCENE next_scene);
bool Scene_Change(void);

#endif // !_SCENE_H_
