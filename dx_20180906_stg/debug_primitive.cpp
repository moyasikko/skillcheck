/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "mydirect3d.h"

typedef struct DebugVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
}DebugVertex;

#define FVF_DEBUG_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

#define CIRCLE_VERTEX_COUNT (8)
#define CIRCLE_DRAW_MAX (2048)

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
static DebugVertex* g_pVertex;
static WORD* g_pIndex;
static int g_CircleCount = 0;




void DebugPrimitive_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	g_CircleCount = 0;

	pDevice->CreateVertexBuffer(sizeof(DebugVertex) * CIRCLE_VERTEX_COUNT * CIRCLE_DRAW_MAX, D3DUSAGE_WRITEONLY, FVF_DEBUG_VERTEX, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * CIRCLE_VERTEX_COUNT * 2 * CIRCLE_DRAW_MAX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);

}
void DebugPrimitive_Finalize(void)
{
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
	if (g_pVertexBuffer)
	{	   
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}
void DebugPrimitive_BatchBegin(void)
{
	g_CircleCount = 0;

	g_pVertexBuffer->Lock(0, 0, (void**)&g_pVertex, 0);
	g_pIndexBuffer ->Lock(0, 0, (void**)&g_pIndex,  0);

}
void DebugPrimitive_BatchDraw(float cx, float cy, float radius, D3DCOLOR color)
{
	int n = g_CircleCount * CIRCLE_VERTEX_COUNT;
	float a = D3DX_PI * 2.0f / CIRCLE_VERTEX_COUNT;
	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++)
	{
		g_pVertex[n + i].position.x = cos(a*i)*radius + cx;
		g_pVertex[n + i].position.y = sin(a*i)*radius + cy;
		g_pVertex[n + i].position.z = 1.0f;
		g_pVertex[n + i].position.w = 1.0f;
		g_pVertex[n + i].color = color;
	}

	n = g_CircleCount * CIRCLE_VERTEX_COUNT * 2;
	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++)
	{
		g_pIndex[n * 2 + i * 2] = n + i;
		g_pIndex[n * 2 + i * 2 + 1] = n + (i + 1) % CIRCLE_VERTEX_COUNT;
	}

	g_CircleCount++;
}
void DebugPrimitive_BatchRun(void)
{
	g_pVertexBuffer->Unlock();
	g_pIndexBuffer->Unlock();

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(DebugVertex));

	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->SetFVF(FVF_DEBUG_VERTEX);
	pDevice->SetTexture(0, NULL);

	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, g_CircleCount * CIRCLE_VERTEX_COUNT, 0, g_CircleCount * CIRCLE_VERTEX_COUNT);
}