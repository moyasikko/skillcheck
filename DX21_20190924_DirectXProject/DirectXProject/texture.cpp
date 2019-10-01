#define _CRT_SECURE_NO_WARNINGS
/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "mydirect3d.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define FILENAME_MAX		(128)
#define TEXTUREDATA_MAX		(64)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct TextureData_tag
{
	char filename[FILENAME_MAX];
	int width;
	int height;
	LPDIRECT3DTEXTURE9 pTexture;
}TextureData;

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
static TextureData g_TextureData[TEXTUREDATA_MAX] = {};


int Texture_SetLoadFile(const char* pFilename, int width, int height)
{
	//�����t�@�C������T��
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (strcmp(pFilename, g_TextureData[i].filename) == 0)
		{//���������ꍇ
			return i;
		}
	}
	//�V�K�o�^
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		//�g�p����Ă��Ȃ��ꏊ��T��
		if (g_TextureData[i].filename[0] != '\0')
		{
			continue;
		}
		//�o�^
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
		
		//�e�N�X�`���[�̓ǂݍ���
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