/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "common.h"
#include <string.h>

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
	D3DXVECTOR2 UV;
}Vertex2D;//座標変換済み頂点　floatの4つ目に1.0fを入れる
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/*******************************************************************************
* グローバル変数
 *******************************************************************************/
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
static D3DCOLOR g_SpriteColor = 0xFFFFFFFF;


#define PI			(3.141592)
#define ROOT2		(1.41421356237)

void Sprite_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);


}
void Sprite_Finalize(void)
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

void Sprite_SetColor(D3DCOLOR color)
{
	g_SpriteColor = color;
}

void Sprite_Draw(int texture_index, float dx, float dy)
{
	int w = Texture_GetWidth(texture_index);
	int h = Texture_GetHeight(texture_index);
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx	 - 0.5f,	dy     - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0, 0) },
		{ D3DXVECTOR4(dx + w - 0.5f,	dy     - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1, 0) },
		{ D3DXVECTOR4(dx	 - 0.5f,	dy + h - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0, 1) },
		{ D3DXVECTOR4(dx + w - 0.5f,	dy + h - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1, 1) }
	};

	MyDirect3D_GetDevice()->SetFVF(FVF_VERTEX2D);

	MyDirect3D_GetDevice()->SetTexture(0, Texture_GetTexture(texture_index));
	MyDirect3D_GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));

}
void Sprite_DrawRotate(int texture_index, float dx, float dy, float f)
{
	int w = Texture_GetWidth(texture_index);
	int h = Texture_GetHeight(texture_index);
	float x[4], y[4];

	x[0] = (w / 2) * cos((-135 + f) * PI / 180.0f) + dx;
	y[0] = (h / 2) * sin((-135 + f) * PI / 180.0f) + dy;
	x[1] = (w / 2) * cos((-45 + f) * PI / 180.0f) + dx;
	y[1] = (h / 2) * sin((-45 + f) * PI / 180.0f) + dy;
	x[2] = (w / 2) * cos((135 + f) * PI / 180.0f) + dx;
	y[2] = (h / 2) * sin((135 + f) * PI / 180.0f) + dy;
	x[3] = (w / 2) * cos((45 + f) * PI / 180.0f) + dx;
	y[3] = (h / 2) * sin((45 + f) * PI / 180.0f) + dy;

	Vertex2D v[] = {
		{ D3DXVECTOR4(x[0] - 0.5f,	y[0] - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0, 0) },
		{ D3DXVECTOR4(x[1] - 0.5f,	y[1] - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1, 0) },
		{ D3DXVECTOR4(x[2] - 0.5f,	y[2] - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0, 1) },
		{ D3DXVECTOR4(x[3] - 0.5f,	y[3] - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1, 1) }
	};

	MyDirect3D_GetDevice()->SetFVF(FVF_VERTEX2D);

	MyDirect3D_GetDevice()->SetTexture(0, Texture_GetTexture(texture_index));
	MyDirect3D_GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));

}
void Sprite_Draw(int texture_index, float dx, float dy, int tx, int ty, int tw, int th)
{
	int w = Texture_GetWidth(texture_index);
	int h = Texture_GetHeight(texture_index);

	float u0 = (float)tx / w;
	float v0 = (float)ty / h;
	float u1 = (float)(tx + tw) / w;
	float v1 = (float)(ty + th) / h;

	Vertex2D v[] = {
		{ D3DXVECTOR4(dx	  - 0.5f,	dy		- 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0) },
		{ D3DXVECTOR4(dx + tw - 0.5f,	dy		- 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0) },
		{ D3DXVECTOR4(dx	  - 0.5f,	dy + th - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1) },
		{ D3DXVECTOR4(dx + tw - 0.5f,	dy + th - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1) }
	};

	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();

	WORD* pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 1;
	pIndex[4] = 3;
	pIndex[5] = 2;

	g_pIndexBuffer->Unlock();

	MyDirect3D_GetDevice()->SetFVF(FVF_VERTEX2D);

	MyDirect3D_GetDevice()->SetTexture(0, Texture_GetTexture(texture_index));
	MyDirect3D_GetDevice()->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));
	MyDirect3D_GetDevice()->SetIndices(g_pIndexBuffer);
	MyDirect3D_GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void Sprite_DrawRotation(int texture_index, float dx, float dy, int tx, int ty, int tw, int th, float cx, float cy, float sx, float sy, float rotation)
{
	int w = Texture_GetWidth(texture_index);
	int h = Texture_GetHeight(texture_index);


	float u0 = (float)tx / w;
	float v0 = (float)ty / h;
	float u1 = (float)(tx + tw) / w;
	float v1 = (float)(ty + th) / h;

	float px[4], py[4];
	px[0] = (w / (w / tw) / ROOT2) * sx * cos((-135 + rotation) * PI / 180.0f);
	py[0] = (h / (h / th) / ROOT2) * sy * sin((-135 + rotation) * PI / 180.0f);
	px[1] = (w / (w / tw) / ROOT2) * sx * cos((-45 +  rotation) * PI / 180.0f);
	py[1] = (h / (h / th) / ROOT2) * sy * sin((-45 +  rotation) * PI / 180.0f);
	px[2] = (w / (w / tw) / ROOT2) * sx * cos((135 +  rotation) * PI / 180.0f);
	py[2] = (h / (h / th) / ROOT2) * sy * sin((135 +  rotation) * PI / 180.0f);
	px[3] = (w / (w / tw) / ROOT2) * sx * cos((45 +   rotation) * PI / 180.0f);
	py[3] = (h / (h / th) / ROOT2) * sy * sin((45 +   rotation) * PI / 180.0f);

	Vertex2D v[] = {
		{ D3DXVECTOR4(px[0] + dx - 0.5f,	py[0] + dy - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0) },
		{ D3DXVECTOR4(px[1] + dx - 0.5f,	py[1] + dy - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0) },
		{ D3DXVECTOR4(px[2] + dx - 0.5f,	py[2] + dy - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1) },
		{ D3DXVECTOR4(px[3] + dx - 0.5f,	py[3] + dy - 0.5f,	1.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1) },
	};

	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();

	WORD* pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 1;
	pIndex[4] = 3;
	pIndex[5] = 2;

	g_pIndexBuffer->Unlock();

	MyDirect3D_GetDevice()->SetFVF(FVF_VERTEX2D);

	MyDirect3D_GetDevice()->SetTexture(0, Texture_GetTexture(texture_index));
	MyDirect3D_GetDevice()->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));
	MyDirect3D_GetDevice()->SetIndices(g_pIndexBuffer);
	MyDirect3D_GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

