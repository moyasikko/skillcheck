#define _CRT_SECURE_NO_WARNINGS
/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "mydirect3d.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define FILENAME_MAX		(128)
#define TEXTUREDATA_MAX		(64)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct TextureData_tag
{
	char filename[FILENAME_MAX];
	int width;
	int height;
	LPDIRECT3DTEXTURE9 pTexture;
}TextureData;

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static TextureData g_TextureData[TEXTUREDATA_MAX] = {};


int Texture_SetLoadFile(const char* pFilename, int width, int height)
{
	//同じファイル名を探す
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (strcmp(pFilename, g_TextureData[i].filename) == 0)
		{//見つかった場合
			return i;
		}
	}
	//新規登録
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		//使用されていない場所を探す
		if (g_TextureData[i].filename[0] != '\0')
		{
			continue;
		}
		//登録
		strcpy(g_TextureData[i].filename, pFilename);
		g_TextureData[i].width = width;
		g_TextureData[i].height = height;

		return i;
	}
	return -1;
}

int Texture_Load(void)
{
	LPDIRECT3DDEVICE9 device = GetDevice();
	int error_count = 0;
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].filename[0] == 0)
		{
			continue;
		}
		if (g_TextureData[i].pTexture != NULL)
		{
			continue;
		}
		
		//テクスチャーの読み込み
		D3DXCreateTextureFromFile(device, g_TextureData[i].filename, &g_TextureData[i].pTexture);
	}
	return error_count;
}

LPDIRECT3DTEXTURE9 Texture_GetTexture(int id)
{
	return g_TextureData[id].pTexture;
}

void Texture_Destroy(int ids[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (g_TextureData[ids[i]].pTexture != NULL)
		{
			g_TextureData[ids[i]].pTexture->Release();
			g_TextureData[ids[i]].pTexture = NULL;
			g_TextureData[ids[i]].filename[0] = 0;
			g_TextureData[ids[i]].width = 0;
			g_TextureData[ids[i]].height = 0;
		}
	}
}

void Texture_Destroy(void)
{
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].pTexture != NULL)
		{
			g_TextureData[i].pTexture->Release();
			g_TextureData[i].pTexture = NULL;
			g_TextureData[i].filename[0] = 0;
			g_TextureData[i].width = 0;
			g_TextureData[i].height = 0;
		}
	}
}

int Texture_GetWidth(int id)
{
	return g_TextureData[id].width;
}

int Texture_GetHeight(int id)
{
	return g_TextureData[id].height;
}