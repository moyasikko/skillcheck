
#include "main.h"
#include "controller.h"
#include "texture.h"
#include "sprite.h"
#include "explosion.h"
#include "score.h"

#define yoko (15)
#define tate (5)
#define block_size (1000.0f / yoko)

unsigned int	g_BlockTexture[2];
struct BLOCK
{
	Float2			Position;
	bool			Flag;
	int				Life;
	bool			Damage;
}g_Block[yoko][tate];

static bool flag;
void InitBlock()
{
	flag = false;
	g_BlockTexture[0] = LoadTexture("rom:/block01.tga");
	g_BlockTexture[1] = LoadTexture("rom:/block02.tga");

	for(int i=0;i<yoko;i++)
		for (int j = 0; j < tate; j++)
		{
			g_Block[i][j].Position.x = block_size * i - (block_size / 2) * (yoko - 1);
			g_Block[i][j].Position.y = (-500 + block_size / 2) + block_size * j;
			g_Block[i][j].Flag = true;
			g_Block[i][j].Damage = true;
			g_Block[i][j].Life = rand() % 2 + 1;
		}
}



void UninitBlock()
{

	UnloadTexture(g_BlockTexture[0]);
	UnloadTexture(g_BlockTexture[1]);

}


void UpdateBlock()
{


	for (int i = 0; i < yoko; i++)
		for (int j = 0; j < tate; j++)
		{
			if (g_Block[i][j].Flag)
			{
				flag = false;
				return;
			}
		}
	flag = true;

}



void DrawBlock()
{

	

	for (int i = 0; i<yoko; i++)
		for (int j = 0; j < tate; j++)
		{
			if (g_Block[i][j].Flag)
			{
				SetTexture(g_BlockTexture[g_Block[i][j].Life - 1]);
				DrawSprite(g_Block[i][j].Position.x, g_Block[i][j].Position.y, block_size, block_size,
					0.0f, 0.0f, 1.0f, 1.0f,
					MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
}

int CollisionBlock(Float2 ball_position)
{
	int hantei = 0;
	for (int i = 0; i < yoko; i++)
		for (int j = 0; j < tate; j++)
		{
			if (g_Block[i][j].Flag)
			{
				if ((g_Block[i][j].Position.y + block_size / 2) >= ball_position.y - 15 &&
					(g_Block[i][j].Position.y - block_size / 2) <= ball_position.y + 15 &&
					(g_Block[i][j].Position.x + block_size / 2) >= ball_position.x - 15 &&
					(g_Block[i][j].Position.x - block_size / 2) <= ball_position.x + 15)
				{
					if(!g_Block[i][j].Damage)
					g_Block[i][j].Life--;
					if (g_Block[i][j].Life == 0)
					{
						g_Block[i][j].Flag = false;
						CreateExplosion(g_Block[i][j].Position);
						CountScore();
					}
					hantei = 2;
					g_Block[i][j].Damage = true;
				}
				else
					g_Block[i][j].Damage = false;
			}
		}

	return hantei;
}

void ReBlock()
{
	for (int i = 0; i<yoko; i++)
		for (int j = 0; j < tate; j++)
		{
			g_Block[i][j].Position.x = block_size * i - (block_size / 2) * (yoko - 1);
			g_Block[i][j].Position.y = (-500 + block_size / 2) + block_size * j;
			g_Block[i][j].Flag = true;
			g_Block[i][j].Damage = true;
			g_Block[i][j].Life = rand() % 2 + 1;
		}
}

bool GetFlagBlock()
{
	return flag;
}