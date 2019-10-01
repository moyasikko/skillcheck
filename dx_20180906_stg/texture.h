#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <d3dx9.h>

#define TEXTURE_INVALID_INDEX (-1)

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
int Texture_SetLoadFile(const char* pFileName, int width, int height);

//�e�N�X�`���[�̓ǂݍ���
//
//�߂�l�F�ǂݍ��߂Ȃ�������
//
int Texture_Load(void);

//�e�N�X�`���[�̎擾
LPDIRECT3DTEXTURE9 Texture_GetTexture(int index);

//�e�N�X�`���[�̕��̎擾
int Texture_GetWidth(int index);

//�e�N�X�`���[�̍����̎擾
int Texture_GetHeight(int index);

//�e�N�X�`���[�̉��
//
//�����Findexes ����������e�N�X�`���[�Ǘ��ԍ����������z��̐擪�A�h���X
//    �Fcount   ����������e�N�X�`���[�Ǘ��ԍ��̌�
int Texture_Release(int indexes[], int count);

//�e�N�X�`���̑S���
void Texture_AllRelease(void);

#endif