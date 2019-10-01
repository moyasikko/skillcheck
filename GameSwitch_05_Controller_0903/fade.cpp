
#include "main.h"
#include "texture.h"
#include "sprite.h"

unsigned int g_Texture;

static float toumeido = 1.0f;
static bool flag = true;
static bool fadeout = false;


void InitFade()
{
	g_Texture = LoadTexture("rom:/fade.tga");
}


void UninitFade()
{
	UnloadTexture(g_Texture);
}


void UpdateFade()
{
	if (flag)
	{
		if (fadeout)
		{
			toumeido += 1.0f / 90.0f;
			if (toumeido >= 1.0f)
			{
				fadeout = false;
				SceneMain();
			}
		}	
		else
		{
			toumeido -= 1.0f / 90.0f;
			if (toumeido <= 0)
			{
				flag = false;
				fadeout = true;
			}
		}
	}
}


void DrawFade()
{
	if (flag)
	{
		SetTexture(g_Texture);

		DrawSprite(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			MakeFloat4(0.0f, 0.0f, 0.0f, toumeido));
	}

}

void FlagFade()
{
	flag = true;
}

bool ReturnFade()
{
	return flag;
}