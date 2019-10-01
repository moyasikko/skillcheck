
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"

#define EXPLOSION_MAX (32)
#define TEXTURE_SIZE (256)
unsigned int g_ExplosionTexture;
struct EXPLOSION
{
	Float2 position;
	Float2 uv;
	int count;
	bool flag;
};

static EXPLOSION g_Explosion[EXPLOSION_MAX];


void InitExplosion()
{
	g_ExplosionTexture = LoadTexture("rom:/explosion.tga");

	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		g_Explosion[i].position.x = 0.0f;
		g_Explosion[i].position.y = 0.0f;
		g_Explosion[i].uv.x = 0.0f;
		g_Explosion[i].uv.y = 0.0f;
		g_Explosion[i].count = 0;
		g_Explosion[i].flag = false;
	}
}


void UninitExplosion()
{
	UnloadTexture(g_ExplosionTexture);
}


void UpdateExplosion()
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (g_Explosion[i].flag)
		{
			g_Explosion[i].uv.x++;
			if (g_Explosion[i].uv.x > 4)
			{
				g_Explosion[i].uv.x = 0;
				g_Explosion[i].uv.y++;
				if (g_Explosion[i].uv.y > 4)
				{
					g_Explosion[i].flag = false;
				}
			}
			g_Explosion[i].count++;
		}
	}
}


void DrawExplosion()
{
	SetTexture(g_ExplosionTexture);
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (g_Explosion[i].flag)
		{
			DrawSprite(
				g_Explosion[i].position.x, g_Explosion[i].position.y,
				TEXTURE_SIZE, TEXTURE_SIZE,
				0.25f * g_Explosion[i].uv.x, 0.25f * g_Explosion[i].uv.y,
				0.25f, 0.25f,
				MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

void CreateExplosion(Float2 b_position)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (!g_Explosion[i].flag)
		{
			g_Explosion[i].position.x = 0.0f;
			g_Explosion[i].position.y = 0.0f;
			g_Explosion[i].uv.x = 0.0f;
			g_Explosion[i].uv.y = 0.0f;
			g_Explosion[i].count = 0;
			g_Explosion[i].flag = true;
			g_Explosion[i].position = b_position;
			PlaySE(WSD_STONE_BREAK1);
			break;
		}
	}
}