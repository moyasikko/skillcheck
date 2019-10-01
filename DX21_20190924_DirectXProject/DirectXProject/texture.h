#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <d3d9.h>

int Texture_SetLoadFile(const char* pFilename, int width, int height);
/*	�ǂݍ��ރe�N�X�`���[�̗\��
	�Ԃ�l�F�\��ԍ�
	pFilename�F�t�@�C����
	width�F��
	height�F����
*/

int Texture_Load(void);
/*	�t�@�C���̓ǂݍ���
	�Ԃ�l�F�ǂݍ��߂Ȃ�������
*/

LPDIRECT3DTEXTURE9 Texture_GetTexture(int id);
/*	�e�N�X�`���[�ԍ��̎擾
	�Ԃ�l�F�e�N�X�`���[�̂���
	id�F�\��ԍ�
*/

void Texture_Destroy(int ids[], int count);
/*	�e�N�X�`���[�̕����J��
	ids[]�F����������e�N�X�`���[��id���������z��̐擪�A�h���X
	count�F�z��̐�
*/

void Texture_Destroy(void);
/*	�e�N�X�`���[�̑S�J��
*/

int Texture_GetWidth(int id);//�e�N�X�`���[�̕��擾
int Texture_GetHeight(int id);//�e�N�X�`���[�̍����擾

#endif // !_TEXTURE_H_
