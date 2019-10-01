/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "effect.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "explosion.h"
#include "debug.h"
#include "mydirect3d.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
struct EFFECT
{
	D3DXVECTOR2 Position;
	float Size;
	D3DCOLOR Color;
	int LifeFrame;
	int SpawnFrame;
};

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
static EFFECT g_Effect[EFFECT_MAX];

static int g_Texture;

static int g_FrameCount = 0;


void Effect_Init(void)
{
	g_Texture = Texture_SetLoadFile("./sozai/img/effect000.jpg", 80, 80);
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effect[i].LifeFrame = 0;
		g_Effect[i].SpawnFrame = -1;
	}
}

void Effect_Uninit(void)
{
	Texture_Destroy(&g_Texture, 1);
}

void Effect_Update(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].LifeFrame == 0)
		{
			continue;
		}
		int ElapsedFrame = g_FrameCount - g_Effect[i].SpawnFrame;
		float e = ElapsedFrame / (float)g_Effect[i].LifeFrame;
		//���͊e�F�Ə�Z�����
		D3DXCOLOR color = g_Effect[i].Color;
		color.a = 1.0f - e;
		g_Effect[i].Color = color;
		if (e > 1.0f)
		{
			e = 1.0f;
			g_Effect[i].LifeFrame = 0;
			g_Effect[i].SpawnFrame = -1;
		}
	}
	g_FrameCount++;
}

void Effect_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// �����_�[�X�e�[�g(���Z��������)
	// �F = (SRC�F * SRC�A���t�@) + (DEST�F * (1.0)�A���t�@)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// �u�����f�B���O����(�f�t�H���g)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// ���f�X�e�B�l�[�V�����J���[�̎w��(1.0)
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].LifeFrame > 0)
		{
			Sprite_SetColor(g_Effect[i].Color);
			Sprite_DrawScale(g_Texture, g_Effect[i].Position.x, g_Effect[i].Position.y, g_Effect[i].Size);
		}
	}
	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	// �F = (SRC�F * SRC�A���t�@) + (DEST�F * (1.0 - SRC)�A���t�@)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// �u�����f�B���O����(�f�t�H���g)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��(1.0 - SRC)
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

}

void Effect_Create(float x, float y, float size, D3DCOLOR color, int lifeframe)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].LifeFrame > 0)
		{
			continue;
		}

		g_Effect[i].Position = D3DXVECTOR2(x, y);
		g_Effect[i].Size = size / 80;
		g_Effect[i].Color = color;
		g_Effect[i].LifeFrame = lifeframe;
		g_Effect[i].SpawnFrame = g_FrameCount;
		break;
	}
}