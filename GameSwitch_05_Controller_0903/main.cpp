
#include "main.h"
#include "controller.h"
#include "texture.h"
#include "sprite.h"
#include "bg.h"
#include "player.h"
#include "ball.h"
#include "block.h"
#include "explosion.h"
#include "score.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "sound.h"

enum
{
	start,
	title,
	game,
	result,
	scene_max
};

void Init();
void Uninit();
void Update();
void Draw();

static int scene = start;
static int scene_old = start;

extern "C" void nnMain()
{
	Init();
	

    while( true )
	{
		if (scene != scene_old)
		{
			Init();
			Uninit();
			scene_old = scene;
			FlagFade();
		}
		Update();
		Draw();
	}
	scene = scene_max;
	Uninit();
}


void Init()
{
	switch (scene)
	{
	case start:
		InitSound();
		InitSystem();
		InitController();
		InitBG();
		InitFade();
		InitTitle();
		//InitPlayer();
		InitBall();
		InitBlock();
		InitExplosion();
		InitScore();
		InitResult();
		scene = title;
		break;
	case title:
		InitTitle();
		break;
	case game:
		InitPlayer();
		InitBall();
		InitBlock();
		InitExplosion();
		InitScore();
		break;
	case result:
		InitResult();
		break;
	}
}


void Uninit()
{
	switch (scene_old)
	{
	case title:
		UninitTitle();
		break;
	case game:
		UninitPlayer();
		UninitBall();
		UninitBlock();
		UninitExplosion();
		UninitScore();
		break;
	case result:
		UninitResult();
		break;
	case scene_max:
		UninitFade();
		UninitBG();
		UninitTitle();
		UninitPlayer();
		UninitBall();
		UninitBlock();
		UninitExplosion();
		UninitScore();
		UninitResult();
		UninitController();
		UninitSystem();
		UninitSound();
		break;
	}
}


void Update()
{
	UpdateController();
	UpdateFade();
	if(!ReturnFade())
	switch (scene)
	{
	case title:
		UpdateTitle();
		break;
	case game:
		UpdateBG();
		UpdatePlayer();
		UpdateBall();
		UpdateBlock();
		UpdateExplosion();
		UpdateScore();
		break;
	case result:
		UpdateResult();
		break;
	}
	UpdateSound();
}


void Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawBG();

	switch (scene)
	{
	case title:
		DrawTitle();
		break;
	case game:
		DrawPlayer();
		DrawBall();
		DrawBlock();
		DrawExplosion();
		DrawScore();
		break;
	case result:
		DrawResult();
		break;
	}
	DrawFade();
	SwapBuffers();
}


void SceneMain()
{
	scene++;
	if (scene == scene_max)
		scene = title;
}
