/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "mydirect3d.h"
#include "texture.h"
#include "common.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define RADIAN			(3.14 / 180)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
//頂点データ構造体（頂点の形（フォーマット）の宣言）
typedef struct Vertex2d_tag
{
	D3DXVECTOR4 position;	//ちょうてんざひょう
	D3DCOLOR color;			//頂点からー
	D3DXVECTOR2 uv;			//UV座標

}Vertex2d;
//FVFの宣言
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//XとYとZとRHWをもつ頂点データを表す

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static D3DCOLOR g_SpriteColor = D3DCOLOR_RGBA(255, 255, 255, 255);
static LPDIRECT3DDEVICE9 g_pDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;


void SpriteInit(void)
{
	g_pDevice = GetDevice();
	g_pDevice->CreateVertexBuffer(sizeof(Vertex2d) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	g_pDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);

	WORD *pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 1;
	pIndex[4] = 3;
	pIndex[5] = 2;

	g_pIndexBuffer->Unlock();
}

void SpriteUninit(void)
{
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
}

void Sprite_Draw(int texId, float dx, float dy)
{
	Vertex2d *pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	Vertex2d v[]=
	{
		{D3DXVECTOR4(dx     - 0.5f, dy     - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4(dx + w - 0.5f, dy     - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(dx     - 0.5f, dy + h - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1.0f, 1.0f)}
	};

	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();
	g_pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2d));
	g_pDevice->SetIndices(g_pIndexBuffer);

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void Sprite_Draw(int texId, float dx, float dy, D3DCOLOR color)
{
	Vertex2d *pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	Vertex2d v[] =
	{
		{D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(1.0f, 1.0f)}
	};

	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();
	g_pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2d));
	g_pDevice->SetIndices(g_pIndexBuffer);

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void Sprite_DrawRotate(int texId, float dx, float dy, float angle)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxRot, mtxTrans, mtxW;
	D3DXMatrixRotationZ(&mtxRot, angle * RADIAN);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxRot * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	Vertex2d v[] =
	{
		{D3DXVECTOR4(-w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4( w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(-w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4( w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1.0f, 1.0f)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawRotate(int texId, float dx, float dy, float angle, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxRot, mtxTrans, mtxW;
	D3DXMatrixRotationZ(&mtxRot, angle * RADIAN);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxRot * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	Vertex2d v[] =
	{
		{D3DXVECTOR4(-w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4( w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(-w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4( w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(1.0f, 1.0f)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawScale(int texId, float dx, float dy, float scale)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxScale, mtxTrans, mtxW;
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	D3DXMatrixScaling(&mtxScale, scale, scale, 1.0f);
	mtxW = mtxScale * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	Vertex2d v[] =
	{
		{D3DXVECTOR4(-w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4( w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(-w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4( w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(1.0f, 1.0f)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawScale(int texId, float dx, float dy, float scale, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxScale, mtxTrans, mtxW;
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	D3DXMatrixScaling(&mtxScale, scale, scale, 1.0f);
	mtxW = mtxScale * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	Vertex2d v[] =
	{
		{D3DXVECTOR4(-w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4( w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(-w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4( w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(1.0f, 1.0f)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawCut(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h)
{
	WORD index[] = { 0,1,2,1,3,2 };
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(dx - cut_w / 2 - 0.5f, dy - cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w / 2 - 0.5f, dy - cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx - cut_w / 2 - 0.5f, dy + cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w / 2 - 0.5f, dy + cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1)}
	};

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawCut(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(dx - cut_w / 2 - 0.5f, dy - cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w / 2 - 0.5f, dy - cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx - cut_w / 2 - 0.5f, dy + cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w / 2 - 0.5f, dy + cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v1)}
	};

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawReverse(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h)
{
	WORD index[] = { 0,1,2,1,3,2 };
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(dx - cut_w / 2 - 0.5f, dy - cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx + cut_w / 2 - 0.5f, dy - cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx - cut_w / 2 - 0.5f, dy + cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1)},
		{D3DXVECTOR4(dx + cut_w / 2 - 0.5f, dy + cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1)}
	};

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawReverse(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(dx - cut_w / 2 - 0.5f, dy - cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx + cut_w / 2 - 0.5f, dy - cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx - cut_w / 2 - 0.5f, dy + cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v1)},
		{D3DXVECTOR4(dx + cut_w / 2 - 0.5f, dy + cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v1)}
	};

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawCR(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxRot, mtxTrans, mtxW;
	D3DXMatrixRotationZ(&mtxRot, angle * RADIAN);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxRot * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(-cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(-cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawCR(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxRot, mtxTrans, mtxW;
	D3DXMatrixRotationZ(&mtxRot, angle * RADIAN);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxRot * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(-cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(-cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawCS(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float scale)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxScale, mtxTrans, mtxW;;
	D3DXMatrixScaling(&mtxScale, scale, scale, 1.0f);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxScale * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(-cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(-cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawCS(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float scale, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxScale, mtxTrans, mtxW;;
	D3DXMatrixScaling(&mtxScale, scale, scale, 1.0f);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxScale * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(-cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(-cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawRS(int texId, float dx, float dy, float angle, float scale)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxRot, mtxScale, mtxTrans, mtxW;
	D3DXMatrixRotationZ(&mtxRot, angle * RADIAN);
	D3DXMatrixScaling(&mtxScale, scale, scale, 1.0f);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxScale * mtxRot * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = 0.0f;
	float v0 = 0.0f;
	float u1 = 1.0f;
	float v1 = 1.0f;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(-w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4( w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(-w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4( w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawRS(int texId, float dx, float dy, float angle, float scale, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxRot, mtxScale, mtxTrans, mtxW;
	D3DXMatrixRotationZ(&mtxRot, angle * RADIAN);
	D3DXMatrixScaling(&mtxScale, scale, scale, 1.0f);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxScale * mtxRot * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = 0.0f;
	float v0 = 0.0f;
	float u1 = 1.0f;
	float v1 = 1.0f;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(-w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4( w / 2 - 0.5f, -h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(-w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4( w / 2 - 0.5f,  h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawCRS(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float scale)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxRot, mtxScale, mtxTrans, mtxW;
	D3DXMatrixRotationZ(&mtxRot, angle * RADIAN);
	D3DXMatrixScaling(&mtxScale, scale, scale, 1.0f);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxScale * mtxRot * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(-cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(-cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_DrawCRS(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float scale, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxRot, mtxScale, mtxTrans, mtxW;
	D3DXMatrixRotationZ(&mtxRot, angle * RADIAN);
	D3DXMatrixScaling(&mtxScale, scale, scale, 1.0f);
	D3DXMatrixTranslation(&mtxTrans, dx, dy, 0.0f);
	mtxW = mtxScale * mtxRot * mtxTrans;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(-cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f, -cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(-cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4( cut_w / 2 - 0.5f,  cut_h / 2 - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_Draw(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float cx, float cy)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxW, mtxR, mtxT, mtxIT;
	D3DXMatrixTranslation(&mtxT, -cx, -cy, 0.0f);
	D3DXMatrixTranslation(&mtxIT, cx,  cy, 0.0f);
	D3DXMatrixRotationZ(&mtxR, angle * RADIAN);
	mtxW = mtxT * mtxR * mtxIT;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(dx         - 0.5f, dy         - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w - 0.5f, dy         - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx         - 0.5f, dy + cut_h - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w - 0.5f, dy + cut_h - 0.5f, 0.0f, 1.0f), g_SpriteColor, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_Draw(int texId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float cx, float cy, D3DCOLOR color)
{
	WORD index[] = { 0,1,2,1,3,2 };
	D3DXMATRIX mtxW, mtxR, mtxT, mtxIT;
	D3DXMatrixTranslation(&mtxT, -cx, -cy, 0.0f);
	D3DXMatrixTranslation(&mtxIT, cx, cy, 0.0f);
	D3DXMatrixRotationZ(&mtxR, angle * RADIAN);
	mtxW = mtxT * mtxR * mtxIT;
	//頂点データ(TRIANGLESTRIP)
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);
	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;
	Vertex2d v[] =
	{
		{D3DXVECTOR4(dx -         0.5f, dy -         0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w - 0.5f, dy -         0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx -         0.5f, dy + cut_h - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w - 0.5f, dy + cut_h - 0.5f, 0.0f, 1.0f), color, D3DXVECTOR2(u1, v1)}
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	g_pDevice->SetFVF(FVF_VERTEX2D);


	g_pDevice->SetTexture(0, Texture_GetTexture(texId));
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));
}

void Sprite_SetColor(D3DCOLOR color)
{
	g_SpriteColor = color;
}