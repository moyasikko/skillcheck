#include "main.h"
#include "controller.h"
#include "texture.h"
#include "sprite.h"
#include "explosion.h"
#include "score.h"
#include "fade.h"
#include "sound.h"


#define yoko (21)
#define tate (13)
#define block_size (1000.0f / yoko)

unsigned int	g_TitleTexture[2];
struct BLOCK
{
	Float2			Position;
	bool			Flag;
	int				Life;
	bool			Damage;
}g_Title[yoko][tate];

static bool flag;
void InitTitle()
{
	flag = false;
	g_TitleTexture[0] = LoadTexture("rom:/block01.tga");
	g_TitleTexture[1] = LoadTexture("rom:/block02.tga");

	for (int i = 0; i<yoko; i++)
		for (int j = 0; j < tate; j++)
		{
			g_Title[i][j].Position.x = block_size * i - (block_size / 2) * (yoko - 1);
			g_Title[i][j].Position.y = (-500 + block_size / 2) + block_size * j;
			g_Title[i][j].Flag = true;
			g_Title[i][j].Damage = true;
			g_Title[i][j].Life = 1;
		}
	g_Title[0 + 1][0 + 1].Life = 2;
	g_Title[0 + 1][1 + 1].Life = 2;
	g_Title[0 + 1][2 + 1].Life = 2;
	g_Title[0 + 1][3 + 1].Life = 2;
	g_Title[0 + 1][4 + 1].Life = 2;
	g_Title[1 + 1][0 + 1].Life = 2;
	g_Title[1 + 1][2 + 1].Life = 2;
	g_Title[1 + 1][4 + 1].Life = 2;
	g_Title[2 + 1][1 + 1].Life = 2;
	g_Title[2 + 1][3 + 1].Life = 2;

	g_Title[4 + 1][0 + 1].Life = 2;
	g_Title[4 + 1][1 + 1].Life = 2;
	g_Title[4 + 1][2 + 1].Life = 2;
	g_Title[4 + 1][3 + 1].Life = 2;
	g_Title[4 + 1][4 + 1].Life = 2;
	g_Title[5 + 1][4 + 1].Life = 2;
	g_Title[6 + 1][4 + 1].Life = 2;

	g_Title[1 + 8][1 + 1].Life = 2;
	g_Title[1 + 8][2 + 1].Life = 2;
	g_Title[1 + 8][3 + 1].Life = 2;
	g_Title[1 + 9][0 + 1].Life = 2;
	g_Title[1 + 9][4 + 1].Life = 2;
	g_Title[1 + 10][1 + 1].Life = 2;
	g_Title[1 + 10][2 + 1].Life = 2;
	g_Title[1 + 10][3 + 1].Life = 2;

	g_Title[12 + 1][1 + 1].Life = 2;
	g_Title[12 + 1][2 + 1].Life = 2;
	g_Title[12 + 1][3 + 1].Life = 2;
	g_Title[13 + 1][0 + 1].Life = 2;
	g_Title[13 + 1][4 + 1].Life = 2;
	g_Title[14 + 1][1 + 1].Life = 2;
	g_Title[14 + 1][3 + 1].Life = 2;

	g_Title[16 + 1][0 + 1].Life = 2;
	g_Title[16 + 1][1 + 1].Life = 2;
	g_Title[16 + 1][2 + 1].Life = 2;
	g_Title[16 + 1][3 + 1].Life = 2;
	g_Title[16 + 1][4 + 1].Life = 2;
	g_Title[17 + 1][1 + 1].Life = 2;
	g_Title[17 + 1][2 + 1].Life = 2;
	g_Title[18 + 1][0 + 1].Life = 2;
	g_Title[18 + 1][3 + 1].Life = 2;
	g_Title[18 + 1][4 + 1].Life = 2;

	g_Title[0 + 1][0 + 7].Life = 2;
	g_Title[0 + 1][1 + 7].Life = 2;
	g_Title[0 + 1][2 + 7].Life = 2;
	g_Title[0 + 1][3 + 7].Life = 2;
	g_Title[0 + 1][4 + 7].Life = 2;
	g_Title[1 + 1][0 + 7].Life = 2;
	g_Title[1 + 1][2 + 7].Life = 2;
	g_Title[1 + 1][4 + 7].Life = 2;
	g_Title[2 + 1][1 + 7].Life = 2;
	g_Title[2 + 1][3 + 7].Life = 2;

	g_Title[0 + 5][0 + 7].Life = 2;
	g_Title[0 + 5][1 + 7].Life = 2;
	g_Title[0 + 5][2 + 7].Life = 2;
	g_Title[0 + 5][3 + 7].Life = 2;
	g_Title[0 + 5][4 + 7].Life = 2;
	g_Title[1 + 5][0 + 7].Life = 2;
	g_Title[1 + 5][2 + 7].Life = 2;
	g_Title[2 + 5][1 + 7].Life = 2;
	g_Title[2 + 5][3 + 7].Life = 2;
	g_Title[2 + 5][4 + 7].Life = 2;

	g_Title[0 + 9][1 + 7].Life = 2;
	g_Title[0 + 9][2 + 7].Life = 2;
	g_Title[0 + 9][3 + 7].Life = 2;
	g_Title[0 + 9][4 + 7].Life = 2;
	g_Title[1 + 9][0 + 7].Life = 2;
	g_Title[1 + 9][2 + 7].Life = 2;
	g_Title[2 + 9][1 + 7].Life = 2;
	g_Title[2 + 9][2 + 7].Life = 2;
	g_Title[2 + 9][3 + 7].Life = 2;
	g_Title[2 + 9][4 + 7].Life = 2;

	g_Title[0 + 13][0 + 7].Life = 2;
	g_Title[0 + 13][1 + 7].Life = 2;
	g_Title[0 + 13][2 + 7].Life = 2;
	g_Title[0 + 13][3 + 7].Life = 2;
	g_Title[0 + 13][4 + 7].Life = 2;
	g_Title[1 + 13][0 + 7].Life = 2;
	g_Title[1 + 13][2 + 7].Life = 2;
	g_Title[1 + 13][4 + 7].Life = 2;
	g_Title[2 + 13][0 + 7].Life = 2;
	g_Title[2 + 13][2 + 7].Life = 2;
	g_Title[2 + 13][4 + 7].Life = 2;

	g_Title[16 + 1][0 + 7].Life = 2;
	g_Title[16 + 1][1 + 7].Life = 2;
	g_Title[16 + 1][2 + 7].Life = 2;
	g_Title[16 + 1][3 + 7].Life = 2;
	g_Title[16 + 1][4 + 7].Life = 2;
	g_Title[17 + 1][1 + 7].Life = 2;
	g_Title[17 + 1][2 + 7].Life = 2;
	g_Title[18 + 1][0 + 7].Life = 2;
	g_Title[18 + 1][3 + 7].Life = 2;
	g_Title[18 + 1][4 + 7].Life = 2;
}

void UninitTitle()
{

	UnloadTexture(g_TitleTexture[0]);
	UnloadTexture(g_TitleTexture[1]);

}

void UpdateTitle()
{

	if (GetControllerTrigger(NpadButton::A::Index))
	{
		FlagFade();
		PlaySE(WSD_DECISION24);
	}

}

void DrawTitle()
{



	for (int i = 0; i<yoko; i++)
		for (int j = 0; j < tate; j++)
		{
			if (g_Title[i][j].Flag)
			{
				SetTexture(g_TitleTexture[g_Title[i][j].Life - 1]);
				DrawSprite(g_Title[i][j].Position.x, g_Title[i][j].Position.y, block_size, block_size,
					0.0f, 0.0f, 1.0f, 1.0f,
					MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
}