/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include <d3dx9.h>
#include <string.h>
#include "texture.h"
#include "mydirect3d.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define TEXTURE_FILENAME_MAX (64)
#define TEXTURE_MANAGE_MAX (256)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
//�e�N�X�`���[�t�@�C�����Ǘ��p���R�[�h�\����
typedef struct TextureFile_tag
{
	char fileName[TEXTURE_FILENAME_MAX];
	int width;
	int height;
}TextureFile;

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
static TextureFile g_Files[TEXTURE_MANAGE_MAX] = {};

static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_MANAGE_MAX] = {};

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
//�ǂݍ��ރe�N�X�`���[�t�@�C���̎��O�\�������
//
//�����FpFileName �t�@�C����
//    �Fwidht     �摜�̕�
//    �Fheight�@�@�摜�̍���
//
//�߂�l�F�\��i�e�N�X�`���[�Ǘ��j�ԍ�
//
int Texture_SetLoadFile(const char* pFileName, int width, int height)
{
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		if (g_Files[i].fileName[0]) continue;

		/*for(int j = 0; j < TEXTURE_MANAGE_MAX; j++)
		{
			if(strcmp(pFileName,g_Files[j].fileName[]))
			//�����t�@�C��������������
				return j;
		}*/

		//�g���Ă��Ȃ����R�[�h
		strcpy(g_Files[i].fileName, pFileName);
		g_Files[i].width = width;
		g_Files[i].height = height;

		return i;
	}
	return TEXTURE_INVALID_INDEX;
}

//�e�N�X�`���[�̓ǂݍ���
//
//�߂�l�F�ǂݍ��߂Ȃ�������
//
int Texture_Load(void)
{
	int failed_count = 0;
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

		
		HRESULT hr;

		if (!g_Files[i].fileName[0]) continue;

		//�f�[�^�x�[�X�ɋL�ڂ���

		if (g_pTextures[i]) continue;

		if (!pDevice)
		{
			failed_count++;
			continue;
		}

		//���S�ɓǂݍ��߂�

		hr = D3DXCreateTextureFromFile(pDevice, g_Files[i].fileName, &g_pTextures[i]);

		if (FAILED(hr))
			failed_count++;
	}
	return failed_count;
}

//�e�N�X�`���[�̎擾
LPDIRECT3DTEXTURE9 Texture_GetTexture(int index)
{
	return g_pTextures[index];
}

//�e�N�X�`���[�̕��̎擾
int Texture_GetWidth(int index)
{
	return g_Files[index].width;
}

//�e�N�X�`���[�̍����̎擾
int Texture_GetHeight(int index)
{
	return g_Files[index].height;
}

//�e�N�X�`���[�̉��
//
//�����Findexes ����������e�N�X�`���[�Ǘ��ԍ����������z��̐擪�A�h���X
//    �Fcount   ����������e�N�X�`���[�Ǘ��ԍ��̌�
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

//�e�N�X�`���̑S���
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
