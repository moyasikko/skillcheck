#ifndef MYDIRECT3D_H_
#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
//mydirect3d�̏�����
bool MyDirect3D_Initialize(HWND hWnd, int screen_x, int screen_y);
//Direvt3D�C���^�[�t�F�C�X����уf�o�C�X�̎擾(Create)

//mydirect3d�̏I������
void MyDirect3D_Finalize(void);
//���2�̉��

//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);
#endif
