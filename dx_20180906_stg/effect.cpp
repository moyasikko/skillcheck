/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3dx9.h>
#include "mydirect3d.h"
#include "texture.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define EFFECT_COUNT_MAX	(8192)

#define FVF_EFFECT			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct Effect_tag
{
	float x, y;
	float radius;
	D3DCOLOR color;
	float alpha;
	int life;
	int barth;
}Effect;
typedef struct EffectVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}EffectVertex;

static Effect g_Effects[EFFECT_COUNT_MAX];
static int g_EffectFrameCount;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
static int g_EffectIndex = TEXTURE_INVALID_INDEX;


void Effect_Initialize()
{
	g_EffectIndex = Texture_SetLoadFile("./img/effect000.jpg", 80, 80);
	g_EffectFrameCount = 0;
	for (int i = 0; i < EFFECT_COUNT_MAX; i++)
	{
		g_Effects[i].life = -1;
	}

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->CreateVertexBuffer(sizeof(EffectVertex) * 4 * EFFECT_COUNT_MAX, D3DUSAGE_WRITEONLY, FVF_EFFECT, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(EffectVertex) * 4 * EFFECT_COUNT_MAX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
}
void Effect_Finalize()
{
	Texture_Release(&g_EffectIndex, 1);
}
void Effect_Update()
{
	for (int i = 0; i < EFFECT_COUNT_MAX; i++)
	{
		if (g_Effects[i].life > 0)
		{
			int age = g_EffectFrameCount - g_Effects[i].barth;
			g_Effects[i].radius *= 0.975f;
			g_Effects[i].alpha = 1.0f - (float)(age / g_Effects[i].life);

			if (age >= g_Effects[i].life)
			{
				g_Effects[i].life = -1;
			}
		}
	}
	g_EffectFrameCount++;
}
void Effect_Draw()
{
	EffectVertex* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);
	WORD* pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	int n = 0;
	for (int i = 0; i < EFFECT_COUNT_MAX; i++)
	{
		if (g_Effects[i].life >= 1)
		{
			for (int j = 0; j < 4; j++)
			{
				pV[n * 4 + j].position.x = cos(D3DXToRadian(45 + 90 * j)) * g_Effects[i].radius + g_Effects[i].x;
				pV[n * 4 + j].position.y = sin(D3DXToRadian(45 + 90 * j)) * g_Effects[i].radius + g_Effects[i].y;
				pV[n * 4 + j].position.z = 1.0f;
				pV[n * 4 + j].position.w = 1.0f;
				pV[n * 4 + j].color = g_Effects[i].color;
			}

			pV[n * 4 + 0].texcoord = D3DXVECTOR2(1.0f, 1.0f);
			pV[n * 4 + 1].texcoord = D3DXVECTOR2(0.0f, 1.0f);
			pV[n * 4 + 2].texcoord = D3DXVECTOR2(0.0f, 0.0f);
			pV[n * 4 + 3].texcoord = D3DXVECTOR2(1.0f, 0.0f);

			pIndex[n * 6 + 0] = 0 + n * 4;
			pIndex[n * 6 + 1] = 1 + n * 4;
			pIndex[n * 6 + 2] = 2 + n * 4;
			pIndex[n * 6 + 3] = 2 + n * 4;
			pIndex[n * 6 + 4] = 3 + n * 4;
			pIndex[n * 6 + 5] = 0 + n * 4;

			n++;
		}
	}
	g_pVertexBuffer->Unlock();
	g_pIndexBuffer->Unlock();

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(EffectVertex));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetFVF(FVF_EFFECT);
	pDevice->SetTexture(0, Texture_GetTexture(g_EffectIndex));

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, n * 4, 0, n * 2);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void Effect_Create(float x, float y, int life, float radius, D3DCOLOR color)
{
	for (int i = 0; i < EFFECT_COUNT_MAX; i++)
	{
		if (g_Effects[i].life < 0)
		{
			g_Effects[i].x = x;
			g_Effects[i].y = y;
			g_Effects[i].radius = radius;
			D3DXCOLOR c = color;
			g_Effects[i].color = color;
			g_Effects[i].alpha = c.a;
			g_Effects[i].life = life;
			g_Effects[i].barth = g_EffectFrameCount;			
			break;
		}
	}
}