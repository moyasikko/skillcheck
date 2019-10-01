/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3dx9.h>
#include <string.h>
#include "texture.h"
#include "mydirect3d.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define TEXTURE_FILENAME_MAX (64)
#define TEXTURE_MANAGE_MAX (256)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
//テクスチャーファイル情報管理用レコード構造体
typedef struct TextureFile_tag
{
	char fileName[TEXTURE_FILENAME_MAX];
	int width;
	int height;
}TextureFile;

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static TextureFile g_Files[TEXTURE_MANAGE_MAX] = {};

static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_MANAGE_MAX] = {};

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
//読み込むテクスチャーファイルの事前予約をする
//
//引数：pFileName ファイル名
//    ：widht     画像の幅
//    ：height　　画像の高さ
//
//戻り値：予約（テクスチャー管理）番号
//
int Texture_SetLoadFile(const char* pFileName, int width, int height)
{
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		if (g_Files[i].fileName[0]) continue;

		/*for(int j = 0; j < TEXTURE_MANAGE_MAX; j++)
		{
			if(strcmp(pFileName,g_Files[j].fileName[]))
			//同じファイル名があったら
				return j;
		}*/

		//使われていないレコード
		strcpy(g_Files[i].fileName, pFileName);
		g_Files[i].width = width;
		g_Files[i].height = height;

		return i;
	}
	return TEXTURE_INVALID_INDEX;
}

//テクスチャーの読み込み
//
//戻り値：読み込めなかった数
//
int Texture_Load(void)
{
	int failed_count = 0;
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

		
		HRESULT hr;

		if (!g_Files[i].fileName[0]) continue;

		//データベースに記載あり

		if (g_pTextures[i]) continue;

		if (!pDevice)
		{
			failed_count++;
			continue;
		}

		//安全に読み込める

		hr = D3DXCreateTextureFromFile(pDevice, g_Files[i].fileName, &g_pTextures[i]);

		if (FAILED(hr))
			failed_count++;
	}
	return failed_count;
}

//テクスチャーの取得
LPDIRECT3DTEXTURE9 Texture_GetTexture(int index)
{
	return g_pTextures[index];
}

//テクスチャーの幅の取得
int Texture_GetWidth(int index)
{
	return g_Files[index].width;
}

//テクスチャーの高さの取得
int Texture_GetHeight(int index)
{
	return g_Files[index].height;
}

//テクスチャーの解放
//
//引数：indexes 解放したいテクスチャー管理番号が入った配列の先頭アドレス
//    ：count   解放したいテクスチャー管理番号の個数
int Texture_Release(int indexes[], int count)
{
	int unrelease_count = 0;
	for (int i = 0; i < count; i++)
	{
		if (g_pTextures[indexes[i]])
		{
			g_pTextures[indexes[i]]->Release();
			g_pTextures[indexes[i]] = NULL;
		}
		else
		{
			unrelease_count++;
		}
		g_Files[indexes[i]].fileName[0] = 0;
	}
	return unrelease_count;
}

//テクスチャの全解放
void Texture_AllRelease(void)
{
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		if (g_pTextures[i])
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
		g_Files[i].fileName[0] = 0;
	}
}
