/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"
#include "mydirect3d.h"
#include "debug_font.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define DEBUG_CIRCLE_VERTEX_NUM (8)
#define DEBUG_CIRCLE_MAX		(4096)

#define FVF_DEBUG_VERTEX		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct DebugCircleVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
}DebugCircleVertex;

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static LPDIRECT3DDEVICE9 g_pDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
static Circle g_Circle[DEBUG_CIRCLE_MAX];
static int g_CircleCount = 0;
static const float A = D3DX_PI * 2.0f / DEBUG_CIRCLE_VERTEX_NUM;
static D3DCOLOR g_color = 0xFFFFFFFF;
static D3DCOLOR g_colors[DEBUG_CIRCLE_MAX];

void Debug_Init(void)
{
	#if defined(_DEBUG) || defined(DEBUG)
		g_pDevice = GetDevice();
		g_pDevice->CreateVertexBuffer(sizeof(DebugCircleVertex) * DEBUG_CIRCLE_MAX * DEBUG_CIRCLE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_DEBUG_VERTEX, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
		g_pDevice->CreateIndexBuffer(sizeof(WORD) * DEBUG_CIRCLE_VERTEX_NUM * 2 * DEBUG_CIRCLE_MAX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
	#endif // _DEBUG || DEBUG
}

void Debug_Uninit(void)
{
#if defined(_DEBUG) || defined(DEBUG)
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
#endif // _DEBUG || DEBUG
}

void Debug_Bigin(void)
{
#if defined(_DEBUG) || defined(DEBUG)
	g_CircleCount = 0;
#endif // _DEBUG || DEBUG
}

void Debug_End(void)
{
#if defined(_DEBUG) || defined(DEBUG)
	DebugCircleVertex *pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	for (int i = 0; i < g_CircleCount; i++)
	{
		int startIndex = i * DEBUG_CIRCLE_VERTEX_NUM;
		for (int j = 0; j < DEBUG_CIRCLE_VERTEX_NUM; j++)
		{
			pV[startIndex + j].position.x = cosf(A * j) * g_Circle[i].radius + g_Circle[i].position.x;
			pV[startIndex + j].position.y = sinf(A * j) * g_Circle[i].radius + g_Circle[i].position.y;
			pV[startIndex + j].position.z = 0.0f;
			pV[startIndex + j].position.w = 1.0f;
			pV[startIndex + j].color = g_colors[i];
		}
	}

	g_pVertexBuffer->Unlock();

	WORD *pI;

	g_pIndexBuffer->Lock(0, 0, (void**)&pI, D3DLOCK_DISCARD);

	for (int i = 0; i < g_CircleCount; i++)
	{
		int startIndex = i * DEBUG_CIRCLE_VERTEX_NUM * 2;
		for (int j = 0; j < DEBUG_CIRCLE_VERTEX_NUM; j++)
		{
			//pI[startIndex + j * 2 + 0] = startIndex + j;
			//pI[startIndex + j * 2 + 1] = startIndex + ((j + 1) % DEBUG_CIRCLE_VERTEX_NUM);
			pI[startIndex + j * 2 + 0] = i * DEBUG_CIRCLE_VERTEX_NUM + j;
			pI[startIndex + j * 2 + 1] = i * DEBUG_CIRCLE_VERTEX_NUM + ((j + 1) % DEBUG_CIRCLE_VERTEX_NUM);
		}
	}

	g_pIndexBuffer->Unlock();
	g_pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(DebugCircleVertex));
	g_pDevice->SetIndices(g_pIndexBuffer);
	g_pDevice->SetFVF(FVF_DEBUG_VERTEX);
	g_pDevice->SetTexture(0, NULL);
	g_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, g_CircleCount * DEBUG_CIRCLE_VERTEX_NUM, 0, g_CircleCount * DEBUG_CIRCLE_VERTEX_NUM);
	//DebugFont_Draw(0, 100, "%d", g_CircleCount);
#endif // _DEBUG || DEBUG
}

void Debug_Draw(float x, float y, float radius)
{
#if defined(_DEBUG) || defined(DEBUG)
	if (g_CircleCount >= DEBUG_CIRCLE_MAX)
	{
		return;
	}
	g_Circle[g_CircleCount].position.x = x;
	g_Circle[g_CircleCount].position.y = y;
	g_Circle[g_CircleCount].radius = radius;
	g_colors[g_CircleCount] = g_color;
	g_CircleCount++;
#endif // _DEBUG || DEBUG
}

void Debug_SetColor(D3DCOLOR color)
{
#if defined(_DEBUG) || defined(DEBUG)
	g_color = color;
#endif // _DEBUG || DEBUG
}