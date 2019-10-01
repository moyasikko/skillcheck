//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000,		    // BGM0
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_POWERUP,		// �p���[�A�b�v��
	SOUND_LABEL_SE_GETITEM,		// �A�C�e���Q�b�g��
	SOUND_LABEL_SE_DAMAGE,		// �_���[�W��
	SOUND_LABEL_SE_WARNING,		// �x����
	SOUND_LABEL_SE_ITAZURA,		// BGM1
	SOUND_LABEL_SE_TITLE,		// BGM2title
	SOUND_LABEL_SE_SUCCESS,		// �N���A
	SOUND_LABEL_SE_GAMEOVER,	// �Q�[���I�[�o�[
	SOUND_LABEL_SE_E_SHOT,		// �G�ʔ���
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
