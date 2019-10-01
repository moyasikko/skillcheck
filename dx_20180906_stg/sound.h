//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000,		    // BGM0
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_POWERUP,		// パワーアップ音
	SOUND_LABEL_SE_GETITEM,		// アイテムゲット音
	SOUND_LABEL_SE_DAMAGE,		// ダメージ音
	SOUND_LABEL_SE_WARNING,		// 警告音
	SOUND_LABEL_SE_ITAZURA,		// BGM1
	SOUND_LABEL_SE_TITLE,		// BGM2title
	SOUND_LABEL_SE_SUCCESS,		// クリア
	SOUND_LABEL_SE_GAMEOVER,	// ゲームオーバー
	SOUND_LABEL_SE_E_SHOT,		// 敵玉発射
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
